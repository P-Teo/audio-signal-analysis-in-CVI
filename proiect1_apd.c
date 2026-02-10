#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include <analysis.h>
#include <math.h>


#include "proiect1_apd.h"

#define SAMPLE_RATE 0
#define NPOINTS 1



static int mainPanel=0;
static int freqPanel = 0;
int waveInfo[2]; //waveInfo[0] = sampleRate //waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 0;
double *waveData = 0;



int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((mainPanel = LoadPanel (0, "proiect1_apd.uir", PANEL)) < 0)
		return -1;
	if ((freqPanel = LoadPanel (0, "proiect1_apd.uir", PANEL_FREQ)) < 0)
		return -1;

	DisplayPanel (mainPanel);
	RunUserInterface ();
	DiscardPanel (mainPanel);
	return 0;
}

int CVICALLBACK OneMainPanel (int panel, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			if (waveData != NULL) 
			{
        		free(waveData);
        		waveData = NULL;
    		}
			QuitUserInterface (0);
			
			break;
	}
	return 0;
}
int CVICALLBACK OnLoadButtonCB (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			//afisare grafic
			
			//executa script python pentru conversia unui fisierului .wav in .txt
			LaunchExecutable("python main.py");
			Delay(4);
			FileToArray("waveInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			waveData = (double *) calloc(npoints, sizeof(double));
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
	
			PlotY(panel,PANEL_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			//pun valorile 
			double minVal=0, maxVal=0, mean=0, median=0, stdDev=0;
        	int idxMin=0, idxMax=0;
			double zeroCrossings = 0;
			MaxMin1D(waveData, npoints, &maxVal, &idxMax, &minVal, &idxMin);
			StdDev(waveData, npoints, &mean, &stdDev);
        	Median (waveData, npoints, &median);
            for (int i = 1; i < npoints; i++)
            {
                if ((waveData[i - 1] <= 0 && waveData[i] > 0) || (waveData[i - 1] >= 0 && waveData[i] < 0))
                    zeroCrossings++;
            }
        	SetCtrlVal (panel, PANEL_NUMERIC_MINIM_VALOARE, minVal);
        	SetCtrlVal (panel, PANEL_NUMERIC_MAXIM_VALOARE, maxVal);
        	SetCtrlVal (panel, PANEL_NUMERIC_MEDIE, mean);
        	SetCtrlVal (panel, PANEL_NUMERIC_MEDIANA, median);
        	SetCtrlVal (panel, PANEL_NUMERIC_DISPERSIE, stdDev);
        	SetCtrlVal (panel, PANEL_NUMERIC_MINIM_INDEX, (double)idxMin);
        	SetCtrlVal (panel, PANEL_NUMERIC_MAXIM_INDEX, (double)idxMax);
			SetCtrlVal(panel, PANEL_NUMERIC_ZERO, zeroCrossings);
			
			
			//histograma

			int numBins = 50;  // nr intervale
			double binWidth = (maxVal - minVal) / numBins;
			double* hist = (double*)calloc(numBins, sizeof(double));
			double* binmij = (double*)malloc(numBins * sizeof(double));
			// frecvente
			for (int i = 0; i < npoints; i++) 
			{
    			int bin = (int)((waveData[i] - minVal) / binWidth);
    			if (bin >= numBins) 
					bin = numBins - 1;
				hist[bin]++;
			}
			for (int i = 0; i < numBins; i++) 
			{
   				binmij[i] = minVal + binWidth * (i + 0.5);
			}
			
			DeleteGraphPlot(panel, PANEL_HISTOGRAMA, -1, VAL_IMMEDIATE_DRAW);
			for (int i = 0; i < numBins; i++)
			{
   				double x1 = binmij[i] - binWidth / 2;
    			double x2 = binmij[i] + binWidth / 2;
    			double y1 = 0;
    			double y2 = hist[i];
    			PlotRectangle(panel, PANEL_HISTOGRAMA, x1, y1, x2, y2, 1,  VAL_RED);  
			}

			
			 //  skewness si kurtosis 
			int N ;
			if (npoints >= 256) 
				N=256 ;
			else
				N=npoints;
			double mean256 = 0.0;
			double std256 = 0.0;
			double m3 = 0.0;
			double m4 = 0.0;
			Mean(waveData, N, &mean256);// media pe primele 256 
			StdDev(waveData, N, &mean256, &std256);// abaterea standard
			Moment(waveData, N, 3, &m3);
			Moment(waveData, N, 4, &m4);
			double skew = 0.0;
			double kurt = 0.0;
			if (std256 > 0)
			{
    			skew = m3 / pow(std256, 3);       
    			kurt = m4 / pow(std256, 4); 
			}
			SetCtrlVal(panel, PANEL_NUMERIC_SKEWNESS, skew);
			SetCtrlVal(panel, PANEL_NUMERIC_KURTOSIS, kurt);
			
    	break;
		
	}
	return 0;
}



int CVICALLBACK AplicaFiltrare (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			if (waveData == NULL || npoints <= 0)
			{
    			MessagePopup("Eroare", "Trebuie sã încarci datele înainte de a aplica filtrul!");
    			return 0;
			}
			SetCtrlVal(panel, PANEL_RING_FILTRU_2, 0);
			SetCtrlAttribute(panel, PANEL_BUTTON_NEXT, ATTR_DIMMED, 1);
            SetCtrlAttribute(panel, PANEL_BUTTON_PREV, ATTR_DIMMED, 1);
            SetCtrlAttribute(panel, PANEL_NUMERIC_START, ATTR_DIMMED, 1);
            SetCtrlAttribute(panel, PANEL_NUMERIC_STOP, ATTR_DIMMED, 1);
			
			DeleteGraphPlot(panel, PANEL_GRAPH_RAW_DATA, -1, VAL_IMMEDIATE_DRAW);
			PlotY(panel, PANEL_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			int startIndex = 0;
            int stopIndex  = npoints - 1;
			int len = stopIndex - startIndex + 1;
			DeleteGraphPlot(panel, PANEL_GRAPH_FILTERED_DATA, -1, VAL_IMMEDIATE_DRAW);
			int filterType = 0;
			GetCtrlVal(panel, PANEL_RING_FILTRU, &filterType);
		
			
			if(filterType==0) // mediere
			{
				double *waveDataMA = (double *) calloc(npoints, sizeof(double));
				int windowSize; // nr puncte pentru mediere
				GetCtrlVal(panel, PANEL_RING_MEDIERE, &windowSize);
				
				for (int i = 0; i < npoints; i++) {
    				double sum = 0;
    				int count = 0;
    				for (int j = i - windowSize / 2; j <= i + windowSize / 2; j++) 
					{
        				if (j >= 0 && j < npoints) 
						{
            				sum += waveData[j];
            				count++;
        				}
    				}
    				waveDataMA[i] = sum / count;
				}
				PlotY(panel, PANEL_GRAPH_FILTERED_DATA, waveDataMA, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			}
			else if(filterType==1)//Ordin I
			{
					double alpha ; // coeficient  (0<alpha<1)
					GetCtrlVal(panel, PANEL_NUMERIC_ALPHA, &alpha);
					double *waveDataI = (double *) calloc(npoints, sizeof(double));
					waveDataI[0] = waveData[0];
					for (int i = 1; i < npoints; i++) 
					{
    					waveDataI[i] = (1 - alpha) * waveDataI[i - 1] + alpha * waveData[i];
					}
					PlotY(panel, PANEL_GRAPH_FILTERED_DATA, waveDataI, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_GREEN);
			}
			else if (filterType == 2) // derivata
            {
				double dt = 1.0 / sampleRate;
                if (len < 2) return 0;
                double *deriv = (double*) calloc(len, sizeof(double));
                for (int i = 1; i < len; i++)
                    deriv[i] = (waveData[startIndex + i] - waveData[startIndex + i - 1])/dt;
                PlotY(panel, PANEL_GRAPH_FILTERED_DATA, deriv, len, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_MAGENTA); 
            }
			break;
	}
	return 0;
}


int CVICALLBACK RingFiltruChanged (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_VAL_CHANGED:
		{
				int filterType = 0;
        		GetCtrlVal(panel, PANEL_RING_FILTRU, &filterType);

        		if (filterType == 0) // mediere
        		{
            		SetCtrlAttribute(panel, PANEL_NUMERIC_ALPHA, ATTR_DIMMED, 1); // dezactivat
            		SetCtrlAttribute(panel, PANEL_RING_MEDIERE, ATTR_DIMMED, 0); // activat
        		}
        		else  if(filterType == 1)// ordin I
        		{
            		SetCtrlAttribute(panel, PANEL_NUMERIC_ALPHA, ATTR_DIMMED, 0); // activat
         		    SetCtrlAttribute(panel, PANEL_RING_MEDIERE, ATTR_DIMMED, 1); // dezactivat
        		}
				else //derivata
				{
					SetCtrlAttribute(panel, PANEL_NUMERIC_ALPHA, ATTR_DIMMED, 1); // dezactivat
            		SetCtrlAttribute(panel, PANEL_RING_MEDIERE, ATTR_DIMMED, 1); // dezactivat
				}
			break;
		}
	}
	return 0;
}

void PlotInterval(int panel)
{
    double startTime, stopTime;
    GetCtrlVal(panel, PANEL_NUMERIC_START, &startTime);
    GetCtrlVal(panel, PANEL_NUMERIC_STOP, &stopTime);

    int startIndex = (int)(startTime * sampleRate);
    int stopIndex  = (int)(stopTime * sampleRate);

    if (startIndex < 0) startIndex = 0;
    if (stopIndex >= npoints) stopIndex = npoints - 1;
    if (stopIndex <= startIndex) stopIndex = startIndex + 1;
    int intervalLength = stopIndex - startIndex + 1;

    // sterg graficele vechi
    DeleteGraphPlot(panel, PANEL_GRAPH_RAW_DATA, -1, VAL_IMMEDIATE_DRAW);
    DeleteGraphPlot(panel, PANEL_GRAPH_FILTERED_DATA, -1, VAL_IMMEDIATE_DRAW);
    // afisez
    PlotY(panel, PANEL_GRAPH_RAW_DATA, &waveData[startIndex], intervalLength, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);

    // aplic filtrarea pt intervalul curent
    int filterType;
    GetCtrlVal(panel, PANEL_RING_FILTRU, &filterType);

    if (filterType == 0) // mediere
    {
        double *waveDataMA = (double *) calloc(intervalLength, sizeof(double));
        int windowSize;
        GetCtrlVal(panel, PANEL_RING_MEDIERE, &windowSize);
        for (int i = 0; i < intervalLength; i++)
        {
            double sum = 0;
            int count = 0;
            for (int j = i - windowSize / 2; j <= i + windowSize / 2; j++)
            {
                int idx = startIndex + j;
                if (idx >= 0 && idx < npoints)
                {
                    sum += waveData[idx];
                    count++;
                }
            }
            waveDataMA[i] = sum / count;
        }
        PlotY(panel, PANEL_GRAPH_FILTERED_DATA, waveDataMA, intervalLength, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
    }
    else if(filterType==1) //cu elem de ordin I
    {
        double alpha;
        GetCtrlVal(panel, PANEL_NUMERIC_ALPHA, &alpha);
        double *waveDataI = (double *) calloc(intervalLength, sizeof(double));
        waveDataI[0] = waveData[startIndex];
        for (int i = 1; i < intervalLength; i++)
        {
            waveDataI[i] = (1 - alpha) * waveDataI[i - 1] + alpha * waveData[startIndex + i];
        }
        PlotY(panel, PANEL_GRAPH_FILTERED_DATA, waveDataI, intervalLength, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_GREEN);
    }
	else if (filterType == 2) // derivata
    {
        if (intervalLength < 2) return;
        double *deriv = (double*) calloc(intervalLength, sizeof(double));
        for (int i = 1; i < intervalLength; i++)
            deriv[i] = waveData[startIndex + i] - waveData[startIndex + i - 1];
        PlotY(panel, PANEL_GRAPH_FILTERED_DATA, deriv, intervalLength, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_MAGENTA); 
    }

    // Salvare jpg
    char fileName[256];
    int bitmapID;
    // Semnal original
    sprintf(fileName, "ProiectAPD\\signal_raw_%03d.jpg", (int)startTime); 
    GetCtrlDisplayBitmap(panel, PANEL_GRAPH_RAW_DATA, 1, &bitmapID);
    SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);
    DiscardBitmap(bitmapID);
    // Semnal filtrat
    sprintf(fileName, "ProiectAPD\\signal_filtered_%03d.jpg", (int)startTime);
    GetCtrlDisplayBitmap(panel, PANEL_GRAPH_FILTERED_DATA, 1, &bitmapID);
    SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);
    DiscardBitmap(bitmapID);

	
}


int CVICALLBACK NextButtonCB(int panel, int control, int event,
                             void *callbackData, int eventData1, int eventData2)
{
    switch(event)
    {
        case EVENT_COMMIT:
        {
            double startTime, stopTime, step = 1.0;
            GetCtrlVal(panel, PANEL_NUMERIC_START, &startTime);
            GetCtrlVal(panel, PANEL_NUMERIC_STOP, &stopTime);

            int stopIndex = (int)((stopTime + step) * sampleRate);
            if (stopIndex >= npoints)
            {
                MessagePopup("Info", "Ai ajuns la capãtul semnalului!");
                return 0;
            }

            startTime += step;
            stopTime  += step;

            SetCtrlVal(panel, PANEL_NUMERIC_START, startTime);
            SetCtrlVal(panel, PANEL_NUMERIC_STOP, stopTime);

            PlotInterval(panel); 
            break;
        }
    }
    return 0;
}

int CVICALLBACK PrevButtonCB(int panel, int control, int event,
                             void *callbackData, int eventData1, int eventData2)
{
    switch(event)
    {
        case EVENT_COMMIT:
        {
            double startTime, stopTime, step = 1.0;
            GetCtrlVal(panel, PANEL_NUMERIC_START, &startTime);
            GetCtrlVal(panel, PANEL_NUMERIC_STOP, &stopTime);

            int startIndex = (int)((startTime - step) * sampleRate);
            if (startIndex < 0)
            {
                MessagePopup("Info", "Ai ajuns la începutul semnalului!");
                return 0;
            }

            startTime -= step;
            stopTime  -= step;

            SetCtrlVal(panel, PANEL_NUMERIC_START, startTime);
            SetCtrlVal(panel, PANEL_NUMERIC_STOP, stopTime);

            PlotInterval(panel); 
            break;
        }
    }
    return 0;
}


int CVICALLBACK ActivareRingChanged (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		
		case EVENT_VAL_CHANGED:
		
			if (waveData == NULL || npoints <= 0)
			{
				SetCtrlVal(panel, control, 0);
    			MessagePopup("Eroare", "Trebuie sã încarci datele înainte de a aplica filtrul!");
    			return 0;
			}
			int activat;
            GetCtrlVal(panel, control, &activat); 

            SetCtrlAttribute(panel, PANEL_BUTTON_NEXT, ATTR_DIMMED, !activat);
            SetCtrlAttribute(panel, PANEL_BUTTON_PREV, ATTR_DIMMED, !activat);
            SetCtrlAttribute(panel, PANEL_NUMERIC_START, ATTR_DIMMED, !activat);
            SetCtrlAttribute(panel, PANEL_NUMERIC_STOP, ATTR_DIMMED, !activat);
			
			if (activat)
			{
				SetCtrlVal(panel, PANEL_NUMERIC_START, 0.0);
				SetCtrlVal(panel, PANEL_NUMERIC_STOP, 1.0);
				PlotInterval(panel);
			}
			else
			{
				DeleteGraphPlot(panel, PANEL_GRAPH_RAW_DATA, -1, VAL_IMMEDIATE_DRAW);
				DeleteGraphPlot(panel, PANEL_GRAPH_FILTERED_DATA, -1, VAL_IMMEDIATE_DRAW);
				if (waveData != NULL && npoints > 0)
                {
                    PlotY(panel, PANEL_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
					AplicaFiltrare(panel, PANEL_BUTTON_APLICA, EVENT_COMMIT, NULL, 0, 0);
                }
			}
			break;
	}
	return 0;
}




int CVICALLBACK OnEnvelopeButtonCB (int panel, int control, int event,
                                    void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
        {
            if (waveData == NULL || npoints <= 0)
            {
                MessagePopup("Eroare", "Trebuie sã încarci datele înainte de a calcula anvelopa!");
                return 0;
            }
            // Verific dacã activ interval este activat
            int activat = 0;
            GetCtrlVal(panel, PANEL_RING_FILTRU_2, &activat); 
            int startIndex = 0;
            int stopIndex = npoints - 1;

            if (activat)
            {
                double startTime, stopTime;
                GetCtrlVal(panel, PANEL_NUMERIC_START, &startTime);
                GetCtrlVal(panel, PANEL_NUMERIC_STOP, &stopTime);

                startIndex = (int)(startTime * sampleRate);
                stopIndex  = (int)(stopTime  * sampleRate);

                if (startIndex < 0) startIndex = 0;
                if (stopIndex >= npoints) stopIndex = npoints - 1;
            }
            int len = stopIndex - startIndex + 1;
         

            // Fereastra 
            double windowSec = 0.002;
            int windowSamples = (int)(windowSec * sampleRate);
            if (windowSamples < 1) windowSamples = 1;
            int halfW = windowSamples / 2;

            double *upper = (double*) calloc(len, sizeof(double));
            double *lower = (double*) calloc(len, sizeof(double));


            // anvelopa
            for (int i = 0; i < len; i++)
            {
                int idx = startIndex + i;
                int s = idx - halfW;
                int e = idx + halfW;
                if (s <= startIndex) s = startIndex;
                if (e >= stopIndex) e = stopIndex;
                double localMax = waveData[s];
                double localMin = waveData[s];

                for (int j = s + 1; j <= e; j++)
                {
                    if (waveData[j] >= localMax) localMax = waveData[j];
                    if (waveData[j] <= localMin) localMin = waveData[j];
                }
                upper[i] = localMax;
                lower[i] = localMin;
            }

            if (activat)
            {
                PlotY(panel, PANEL_GRAPH_RAW_DATA, upper, len, VAL_DOUBLE, VAL_THIN_LINE, VAL_NO_POINT, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLACK);
                PlotY(panel, PANEL_GRAPH_RAW_DATA, lower, len, VAL_DOUBLE, VAL_THIN_LINE, VAL_NO_POINT, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
            }
            else
            {
                PlotY(panel, PANEL_GRAPH_RAW_DATA, upper, len, VAL_DOUBLE, VAL_THIN_LINE, VAL_NO_POINT, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLACK);
                PlotY(panel, PANEL_GRAPH_RAW_DATA, lower, len, VAL_DOUBLE, VAL_THIN_LINE, VAL_NO_POINT, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
            }
            break;
        }
    }
    return 0;
}



int CVICALLBACK OnSwitchPanelCB(int panel, int control, int event,
                                void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_COMMIT)
    {
        if(panel == mainPanel)
        {
            SetCtrlVal(freqPanel, PANEL_SWITCH_PANEL, 1);  
            DisplayPanel(freqPanel);
            HidePanel(mainPanel);
        }
        else if(panel == freqPanel)
        {
            SetCtrlVal(mainPanel, PANEL_SWITCH_PANEL, 0); 
            DisplayPanel(mainPanel);
            HidePanel(freqPanel);
        }
    }
    return 0;
}






