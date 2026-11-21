# ECG-CPP

ECG heartbeat detection and classification pipeline written in pure C++, built on the MIT-BIH Arrhythmia Database from PhysioNet. 
The pipeline reads raw WFDB records, filters the signal with a bandpass filter and a 60 Hz notch, and detects R-peaks with the Pan-Tompkins algorithm. 
Each detected beat is segmented and turned into features such as RR intervals and waveform shape, and a logistic regression classifier labels it as 
normal or abnormal. Models are evaluated with a patient-wise train/test split and reported with per-class recall rather than accuracy alone, since 
abnormal beats are rare. The project is written in C++ because real-time wearable and embedded medical devices run on C/C++, and it is designed to be 
ported to a microcontroller with an ECG front end in a later phase.
