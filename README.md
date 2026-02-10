# 🎧 Audio Signal Analysis in CVI / Achiziția și Prelucrarea Datelor

**EN:** Academic project for audio signal acquisition, processing, and analysis in time and frequency domain using LabWindows/CVI.


---

## 📌 Descriere proiect
Proiectul constă în realizarea unei aplicații pentru **analiza semnalelor audio în domeniul timp și frecvență**, utilizând **NI LabWindows/CVI** pe sistem de operare Windows.  
Aplicația permite încărcarea unui fișier audio de tip WAV și afișarea semnalului pe un control grafic.

- În domeniul **timp**, aplicația realizează filtrarea semnalului prin **mediere** și **filtru de ordinul I**, cu parametru **alpha ajustabil**, afișând semnalul filtrat și anvelopa acestuia.  
- Analiza se efectuează pe **ferestre de câte o secundă**, pentru care se calculează **skewness** și **kurtosis**.  
- În domeniul **frecvență**, aplicația calculează spectrul de putere utilizând funcțiile LabWindows/CVI.  
- Graficele obținute sunt salvate automat ca **imagini JPEG**, evidențiind eficiența metodelor de prelucrare a semnalelor audio.

---

## 🛠 Cerințe, fișier utilizat și mediu de dezvoltare
- Proiectul are ca obiectiv implementarea unei **interfețe grafice** pentru analiza complexă a semnalelor audio.  
- Fișierul utilizat: **27.wav**, 4 secunde.  
- La apăsarea butonului **Load**, fișierul `.wav` este convertit în:
  - `waveInfo.txt` – rata de eșantionare și număr total de puncte  
  - `waveData.txt` – valorile eșantioanelor  
- **Mediu de dezvoltare principal:** NI LabWindows/CVI (ANSI C)  
- **Suplimentar:** Python 3.8 pentru conversia și preprocesarea datelor  

---

## ⚙️ Etapa 1: Analiza în domeniul timp


### 🔹 Încărcarea și vizualizarea semnalului
- Script Python (`main.py`) convertește datele binare din `.wav`.  
- Interfața permite vizualizarea întregului semnal sau pe ferestre de 1 secundă.  
- Navigare prin semnal: **Next / Prev**.
<img src="https://github.com/user-attachments/assets/e3dec5be-8e8f-4492-82f5-71aaabd26783" alt="Vizualizare semnal WAV" width="700"/>


### 🔹 Analiza statistică
- Calcul automat al parametrilor imediat după încărcare:
  - **Zero Crossing Rate** (număr de treceri prin zero)  
  - **Histogramă amplitudini**  
- Calcul **Skewness** și **Kurtosis** pentru primele 256 de eșantioane.
<img src="https://github.com/user-attachments/assets/94318fa5-6abd-4669-8a71-1a060091c3e3" alt="Histogramă și parametri statistici" width="700"/>

### 🔹 Filtrarea semnalului și derivata
- **Filtru de mediere**: media pe 16 sau 32 de puncte (32 netezește mai mult semnalul).  
- **Filtru de ordinul I**: `filt[i] = (1 - alpha) * filt[i-1] + alpha * signal[i]`  
  - Alpha recomandat: 0.1–0.2 pentru semnale cu variații rapide.  
- **Derivata semnalului**: evidențiază variațiile bruște de amplitudine.
<img src="https://github.com/user-attachments/assets/034b494c-ba1e-4657-ad51-17c5bc8a0a4a" alt="Filtrare și derivata semnalului" width="700"/>

### 🔹 Extracția anvelopei
- Algoritm pentru **localMax** și **localMin** în fiecare fereastră.  
- Rezultatul se afișează peste semnalul original:
  - **Negru** – anvelopa superioară  
  - **Albastru** – anvelopa inferioară  
<img src="https://github.com/user-attachments/assets/0f06efc5-a18a-4df3-a73c-a71d382e2c63" alt="Extracția anvelopei semnalului" width="700"/>

### 💾 Salvarea rezultatelor
- Fiecare cadru analizat (1 secundă) se salvează automat ca **JPEG**.  
- Numele fișierelor generate dinamic: ex. `signal_raw_001.jpg`.  

---

## ⚙️ Etapa 2: Analiza în frecvență

### 🔹 Analiza spectrală
- Transformarea semnalului din **timp** în **frecvență** folosind **Advanced Analysis Library**.  
- Proces automat cu **Timer**, ferestre de dimensiune selectabilă (1024, 2048 etc.).  
- Ferestre aplicate: **Hanning** sau **Hamming**  
  - Hamming reduce zgomotul de fond în jurul peak-urilor.

### 🔹 Filtrarea digitală în frecvență
Filtre band-pass (900 Hz – 1100 Hz):  
- **Chebyshev Ordin I (Ch_BPF)**: pantă abruptă, dar ondulații în banda de trecere  
- **Chebyshev Ordin II (InvCh_BPF)**: bandă de trecere plată, ondulații în banda de stopare  

- Rezultatele sunt afișate simultan:
  - Semnal original vs. semnal filtrat  
  - Spectru original vs. spectru filtrat  
<table>
<tr>
  <td align="center">
    <img src="https://github.com/user-attachments/assets/32fc5912-171d-44be-afde-1695273141a2" alt="Ordin 1" width="500"/><br/>
    Ordin 1
  </td>
  <td align="center">
    <img src="https://github.com/user-attachments/assets/12e4b51e-df43-4435-952f-dbd2beaf727d" alt="Ordin 2" width="500"/><br/>
    Ordin 2
  </td>
</tr>
</table>







### 💾 Salvare rezultate frecvență
- Funcție screenshot pentru toate graficele simultan  
- Numele fișierelor include **data și ora exactă**  

---

## ✅ Concluzii
- Aplicația este **stabilă**, permite navigarea secundă cu secundă prin semnal.  
- Oferă **feedback vizual instantaneu** prin grafice și indicatori numerici.  
- Funcția de salvare automată ca **JPEG** permite documentarea rapidă a oricărui set de date audio.  

---


## ▶️ Rulare proiect
1. Deschide proiectul în **LabWindows/CVI**  
2. Rulează aplicația  
3. Încarcă fișier WAV folosind **Load**  
4. Aplică filtre și navighează prin semnal  
5. Vizualizează analiza în **timp și frecvență**


---

## 🧾 *License*

This project was created for **educational purposes**. 
