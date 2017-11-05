# EQTime
EQTime is An automated seismic data processing tool, featuring fast travel time acquisition and other measurements, including waveform distortion and attenuation. It is designed to automatically process large mount of seismic data and extract useful information systematically and objectively.


This core code of this tool is Empirical Wavelet Construction Algorithm.
Empirical Wavelet Algorithm is an algorithm to process seismic data on the base
of each individual earthquake. It constructs an Empirical Wavelet that best
represent the characteristic of earthquake source without any prior information
about the earthquake source. Each seismography is usually convolution result of
earthquake source, earth structure along the way through seismic wave
propagation path and seismic station instrument response. Since earth structure
is the result we seek to resolve and earthquake source is usually very complex
and hard to characterize, Empirical Wavelet is very helpful to help us define an
empirical waveform that best characterize the earthquake source. The details
about Empirical Waveform construction are described in Method section.

