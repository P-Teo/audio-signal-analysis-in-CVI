#include <analysis.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <utility.h>
#include <advanlys.h>
#include "proiect1_apd.h"
#include "Frequency.h"




extern double *waveData;
extern int npoints;
extern double sampleRate;

double *autoSpectrum = NULL;
double *convertedSpectrum = NULL;
double *freqAxis = NULL;
int n,filter;
double df; //rezolutia în frecventa
double dt; //pas
static int freqCurrentIndex = 0;
double *buffer = NULL;
double *filteredBuffer = NULL;


double frequencyPeak;
double powerPeak;

WindowConst winConst;
char unit[32000] = "V";


int CVICALLBACK OnFreqPanelCB (int panel, int event, void *callbackData,
							   int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			SetCtrlAttribute(panel, PANEL_FREQ_TIMER, ATTR_ENABLED, 0);
			QuitUserInterface (0);
			break;
	}
	return 0;
}




int CVICALLBACK OnTimer (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_TIMER_TICK:
            if (waveData == NULL) return 0;
            
            GetCtrlVal(panel, PANEL_FREQ_NUMERIC, &n);

            if (buffer) free(buffer);
            if (filteredBuffer) free(filteredBuffer);
            if (autoSpectrum) free(autoSpectrum);
            if (convertedSpectrum) free(convertedSpectrum);
            if (freqAxis) free(freqAxis);
			
            buffer = malloc(n * sizeof(double));
            filteredBuffer = malloc(n * sizeof(double));
            autoSpectrum = malloc((n/2) * sizeof(double));
            convertedSpectrum = malloc((n/2) * sizeof(double));
            freqAxis = malloc((n/2) * sizeof(double));

			
            if (freqCurrentIndex + n > npoints) 
				freqCurrentIndex = 0;
            memcpy(buffer, &waveData[freqCurrentIndex], n * sizeof(double));
            freqCurrentIndex += n;

            //  DATE PENTRU SPECTRU 
            dt = 1.0 / sampleRate;
            int winType=1;
            GetCtrlVal(panel, PANEL_FREQ_RING, &winType);
            int cviWindow ;
			if (winType == 1)
			{
				cviWindow = HANNING_;
			} 
			else 
			{
				cviWindow = HAMMING_;
			}

            // GRAFIC 1 & 2: SEMNAL ORIGINAL sI SPECTRUL  
            double *tempBuffer = malloc(n * sizeof(double));
            memcpy(tempBuffer, buffer, n * sizeof(double));
            
            ScaledWindowEx(tempBuffer, n, cviWindow, 0.0, &winConst);
            AutoPowerSpectrum(tempBuffer, n, dt, autoSpectrum, &df);
			PowerFrequencyEstimate(autoSpectrum, n/2, -1.0, winConst, df, 7, &frequencyPeak, &powerPeak);
            SpectrumUnitConversion(autoSpectrum, n/2, SPECTRUM_POWER, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum, unit);
            
			SetCtrlVal(panel, PANEL_FREQ_NUMERIC_FREQ_2, frequencyPeak);
            SetCtrlVal(panel, PANEL_FREQ_NUMERIC_AMP_2, powerPeak);
			
            for (int i = 0; i < n/2; i++) 
				freqAxis[i] = i * df; //vector frecvente

            DeleteGraphPlot(panel, PANEL_FREQ_GRAPH_SEMNAL, -1, VAL_IMMEDIATE_DRAW);
            DeleteGraphPlot(panel, PANEL_FREQ_GRAPH_SPECTRU_SEMNAL, -1, VAL_IMMEDIATE_DRAW);
            PlotY(panel, PANEL_FREQ_GRAPH_SEMNAL, buffer, n, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
            PlotXY(panel, PANEL_FREQ_GRAPH_SPECTRU_SEMNAL, freqAxis, convertedSpectrum, n/2, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
            
            free(tempBuffer);

            // FILTRARE 
            GetCtrlVal(panel, PANEL_FREQ_RING_2, &filter); 
			//Chebyshev de ordin I si II, trece banda (900 - 1100 Hz)
            if (filter == 1) 
				Ch_BPF(buffer, n, sampleRate, 900.0, 1100.0, 0.1, 5, filteredBuffer);
            else if (filter == 2) 
				InvCh_BPF(buffer, n, sampleRate, 900.0, 1100.0, 40.0, 5, filteredBuffer);
			
			
            // GRAFIC 3 & 4: SEMNAL FILTRAT SI SPECTRUL 
            ScaledWindowEx(filteredBuffer, n, cviWindow, 0.0, &winConst);
            AutoPowerSpectrum(filteredBuffer, n, dt, autoSpectrum, &df);
            PowerFrequencyEstimate(autoSpectrum, n/2, -1.0, winConst, df, 7, &frequencyPeak, &powerPeak);
            SpectrumUnitConversion(autoSpectrum, n/2, SPECTRUM_POWER, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum, unit);

            DeleteGraphPlot(panel, PANEL_FREQ_GRAPH_SEMNAL_FILTRAT, -1, VAL_IMMEDIATE_DRAW);
            DeleteGraphPlot(panel, PANEL_FREQ_GRAPH_SPECTRU_FILTRAT, -1, VAL_IMMEDIATE_DRAW);
            PlotY(panel, PANEL_FREQ_GRAPH_SEMNAL_FILTRAT, filteredBuffer, n, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_GREEN);
            PlotXY(panel, PANEL_FREQ_GRAPH_SPECTRU_FILTRAT, freqAxis, convertedSpectrum, n/2, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_BLUE);

            SetCtrlVal(panel, PANEL_FREQ_NUMERIC_FREQ, frequencyPeak);
            SetCtrlVal(panel, PANEL_FREQ_NUMERIC_AMP, powerPeak);
            
            break;
    }
    return 0;
}




int CVICALLBACK OnStart (int panel, int control, int event,
                                  void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            int val;
            GetCtrlVal(panel, PANEL_FREQ_TOGGLEBUTTON, &val); 
            
            if (val) 
			{
                SetCtrlAttribute(panel, PANEL_FREQ_TIMER, ATTR_ENABLED, 1);
            } 
			else
			{
                SetCtrlAttribute(panel, PANEL_FREQ_TIMER, ATTR_ENABLED, 0);
            }
            break;
    }
    return 0;
}


int CVICALLBACK Save_graph (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			int imghandle;
			char fileName[260];
			int ore, min, sec;
			int luna, zi, an;
			
			GetSystemTime(&ore, &min, &sec);
			GetSystemDate(&luna, &zi, &an);
			
			sprintf(fileName, "POZE\\Semnal_%02d-%02d-%04d_%02d-%02d-%02d.jpg", zi, luna, an, ore, min, sec);
			GetCtrlDisplayBitmap(panel, PANEL_FREQ_GRAPH_SEMNAL, 1, &imghandle);
			SaveBitmapToJPEGFile(imghandle, fileName, JPEG_PROGRESSIVE, 100);
			DiscardBitmap(imghandle);
			
			sprintf(fileName, "POZE\\Spectru_%02d-%02d-%04d_%02d-%02d-%02d.jpg", zi, luna, an, ore, min, sec);
			GetCtrlDisplayBitmap(panel, PANEL_FREQ_GRAPH_SPECTRU_SEMNAL, 1, &imghandle);
			SaveBitmapToJPEGFile(imghandle, fileName, JPEG_PROGRESSIVE, 100);
			DiscardBitmap(imghandle);
			
			sprintf(fileName, "POZE\\Filtrat_%02d-%02d-%04d_%02d-%02d-%02d.jpg", zi, luna, an, ore, min, sec);
			GetCtrlDisplayBitmap(panel, PANEL_FREQ_GRAPH_SEMNAL_FILTRAT, 1, &imghandle);
			SaveBitmapToJPEGFile(imghandle, fileName, JPEG_PROGRESSIVE, 100);
			DiscardBitmap(imghandle);
			
			sprintf(fileName, "POZE\\Spectru_Filtrat_%02d-%02d-%04d_%02d-%02d-%02d.jpg", zi, luna, an, ore, min, sec);
			GetCtrlDisplayBitmap(panel, PANEL_FREQ_GRAPH_SPECTRU_FILTRAT, 1, &imghandle);
			SaveBitmapToJPEGFile(imghandle, fileName, JPEG_PROGRESSIVE, 100);
			DiscardBitmap(imghandle);

			break;
	}
	return 0;
}