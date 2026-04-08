# Heap Buffer Overflow Project
<img width="1160" height="583" alt="image" src="https://github.com/user-attachments/assets/a7a3fc47-db4d-4dda-b84d-6da65236478a" />

## Introduzione
Il progetto si ispira a vulnerabilità reali come **Windows SPNEGO heap overflow** (`CVE-2025-47981`), in cui un errore nella fase di parsing di input di rete porta a un heap buffer overflow.

L’idea è quella di estrarre il concetto chiave implementando un server vulnerabile che utilizza un mini-protocollo con campi `TYPE` e `LENGTH`, simulando il comportamento dei protocolli reali.

Il server riceve un campo `LENGTH` controllato dall’utente e lo utilizza direttamente per copiare dati in un buffer heap senza verificare la dimensione. Questo permette di causare un heap overflow e di sovrascrivere un puntatore a funzione, controllando così il flusso di esecuzione.

## Possibili scenari
Gli scenari che si vogliono analizzare sono:
* Senza protezioni
* Con protezioni attive
* Con protezioni disabilitate ma introducendo il fix (bounds checking)

## Analisi con IDS
<img width="704" height="380" alt="image" src="https://github.com/user-attachments/assets/7cc5316b-22bc-4014-b3bd-27bd837eac70" />

Il progetto include una fase di osservazione tramite sistemi di intrusion detection:
* **Snort (NIDS)** per analizzare il traffico di rete e rilevare pattern associati all’exploit

### Opzionale
* **Falco (HIDS)** per monitorare il comportamento del sistema durante l’attacco e confrontare payload standard e offuscati

## Obiettivi del progetto
* Analizzare il comportamento di una vulnerabilità di heap buffer overflow in un contesto controllato
* Osservare l’impatto delle principali mitigazioni di sicurezza sullo sfruttamento della vulnerabilità
* Valutare l’efficacia dei sistemi IDS nel rilevare attacchi (anche offuscati)
* Evidenziare come la validazione dell’input prevenga queste vulnerabilità
