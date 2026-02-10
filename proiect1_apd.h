/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: OneMainPanel */
#define  PANEL_GRAPH_FILTERED_DATA        2       /* control type: graph, callback function: (none) */
#define  PANEL_GRAPH_RAW_DATA             3       /* control type: graph, callback function: (none) */
#define  PANEL_BUTTON_LOAD                4       /* control type: command, callback function: OnLoadButtonCB */
#define  PANEL_SWITCH_PANEL               5       /* control type: binary, callback function: OnSwitchPanelCB */
#define  PANEL_BUTTON_ANVELOPA            6       /* control type: command, callback function: OnEnvelopeButtonCB */
#define  PANEL_BUTTON_APLICA              7       /* control type: command, callback function: AplicaFiltrare */
#define  PANEL_BUTTON_NEXT                8       /* control type: command, callback function: NextButtonCB */
#define  PANEL_BUTTON_PREV                9       /* control type: command, callback function: PrevButtonCB */
#define  PANEL_RING_FILTRU_2              10      /* control type: ring, callback function: ActivareRingChanged */
#define  PANEL_RING_FILTRU                11      /* control type: ring, callback function: RingFiltruChanged */
#define  PANEL_HISTOGRAMA                 12      /* control type: graph, callback function: (none) */
#define  PANEL_NUMERIC_DISPERSIE          13      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_MEDIANA            14      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_MEDIE              15      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_MAXIM_INDEX        16      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_STOP               17      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_START              18      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_KURTOSIS           19      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_SKEWNESS           20      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_MINIM_VALOARE      21      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_MAXIM_VALOARE      22      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_MINIM_INDEX        23      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_ZERO               24      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_ALPHA              25      /* control type: numeric, callback function: (none) */
#define  PANEL_RING_MEDIERE               26      /* control type: ring, callback function: (none) */
#define  PANEL_DECORATION_3               27      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_7               28      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_6               29      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_5               30      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_4               31      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION                 32      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_2               33      /* control type: deco, callback function: (none) */

#define  PANEL_FREQ                       2       /* callback function: OnFreqPanelCB */
#define  PANEL_FREQ_DECORATION_9          2       /* control type: deco, callback function: (none) */
#define  PANEL_FREQ_GRAPH_SPECTRU_FILTRAT 3       /* control type: graph, callback function: (none) */
#define  PANEL_FREQ_GRAPH_SEMNAL_FILTRAT  4       /* control type: graph, callback function: (none) */
#define  PANEL_FREQ_SWITCH_PANEL          5       /* control type: binary, callback function: OnSwitchPanelCB */
#define  PANEL_FREQ_GRAPH_SPECTRU_SEMNAL  6       /* control type: graph, callback function: (none) */
#define  PANEL_FREQ_GRAPH_SEMNAL          7       /* control type: graph, callback function: (none) */
#define  PANEL_FREQ_DECORATION_8          8       /* control type: deco, callback function: (none) */
#define  PANEL_FREQ_RING_2                9       /* control type: ring, callback function: (none) */
#define  PANEL_FREQ_RING                  10      /* control type: ring, callback function: (none) */
#define  PANEL_FREQ_NUMERIC               11      /* control type: numeric, callback function: (none) */
#define  PANEL_FREQ_NUMERIC_AMP_2         12      /* control type: numeric, callback function: (none) */
#define  PANEL_FREQ_NUMERIC_FREQ_2        13      /* control type: numeric, callback function: (none) */
#define  PANEL_FREQ_NUMERIC_AMP           14      /* control type: numeric, callback function: (none) */
#define  PANEL_FREQ_NUMERIC_FREQ          15      /* control type: numeric, callback function: (none) */
#define  PANEL_FREQ_TIMER                 16      /* control type: timer, callback function: OnTimer */
#define  PANEL_FREQ_TOGGLEBUTTON          17      /* control type: textButton, callback function: OnStart */
#define  PANEL_FREQ_BUTTON_SAVE           18      /* control type: command, callback function: Save_graph */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK ActivareRingChanged(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK AplicaFiltrare(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK NextButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OneMainPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnEnvelopeButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFreqPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnStart(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchPanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PrevButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RingFiltruChanged(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Save_graph(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif