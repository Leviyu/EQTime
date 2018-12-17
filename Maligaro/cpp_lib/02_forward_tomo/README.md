

## Iterative Forward Tomography


The goal of this code is to implement iteratively forward tomography algorithm, this code is written with cpp and this is a document on the objective, data structure and processing flow of this algorithm.



#### Objective

Global seismic tomography usually process global seismic data and produce a there dimensional model of Earth`s mantle structure. This is usually achieved by implementing an inversion where a best fitting model is chosen to best explain the seismic data set. Global seismic tomography is very powerful in a way that they compiled global seismic data and aims at resolving global mantle structure. Global tomography provides us an intuitive way to understand global mantle structure without going into the immense details of each seismogram. To achieve this goal, global seismic tomography usually include huge mount of data and often use different data types to achieve better spatial coverage or frequency coverage.

As more data and different data types are included in global tomography, the large-scale mantle structures are well constrained. For lower mantle structures, two large low shear velocity provinces (LLSVPs) are confirmed, one is beneath the Pacific Ocean and the other is beneath Africa. And between these two LLSVPs is a high velocity band, which is around the Pacific Rim. Our understanding about LLSVPs is important as LLSVPs are very large-scale structures that could help us constrain the dynamics and evolution of Earth`s interior, our understanding on LLSVPs has been improved in various ways, seismic studies have been conducted to constrain the scale, shape and velocity contrast, numerous studies have focused especially on LLSVP edges through travel time analysis and waveform modeling (Quote#1). A few studies have been conducted to constrain LLSVP tops (Quote#2). Also several studies (Quote#3) have tried to compare different tomography models and aims at constrain the robust features of LLSVPs. Global tomography is currently the best way to represent the through features of LLSVPs, however tomography models are different to each other as they are often derived using different datasets, parameterizations and theory behind the approximations used in the inversion. These studies have shown that different global tomography models generally agree on large-scale features, especially related to LLSVPs, but the intermediate to small-scale features are not consistent between different models. 

Further more, global seismic tomography is usually done through inversion of huge amount of data. The nature of inversion has determined that the final model are chosen to be the model that best fit the whole dataset with the least variation relative to the reference model. This means the amplitude of tomography models are usually damped and the features in tomography models are usually smoothed. 

However several studies (Quote#) have suggested that LLSVPs might be thermal-chemical and thus the lower mantle structures can be sharp (big velocity gradient). This is confirmed by seismic studies showing LLSVP boundaries are sharp. To further explore the possible sharp features within LLSVPs, in high velocity regions beneath the Pacific Rim and in the background mantle between high velocity band and LLSVPs, we propose this iterative forward tomography approach.

The iterative forward tomography approach serves to restore the sharp features that are heavily damped/smoothed by tomography through inversion. We used high frequency travel time data collected by our Empirical Wavelet Method and compare the data with the travel time prediction from a starting tomography model. We minimize this travel time difference between data and prediction by incorporating the data into tomography models by distributing the residual travel time on the ray-path of each record and updating the tomography model by converting the travel time residual into residual velocity perturbation within each cell. 

This is done in a iteratively way to make sure the updated tomography model best explains our dataset and in each iteration the model is updated layer by layer from surface to CMB (core mantle boundary). This step is implemented to make sure that shallow phases (phases with shallow turning depth) do not affect deeper structures. 


With this iterative forward approach, we seek to:
```
1.	Restore the sharp features that is damped/smoothed by tomography through inversion
2.	Explore the several low velocity regions other then LLSVPs
3.	Resolve the internal velocity variation of LLSVPs
4.	Constrain the LLSVP tops
5.	Update tomography models to achieve uniform/similar 3D model of earth`s mantle structure 
```

*The detailed procedure of iterative forward tomography approach is explained in the paper draft.


#### Data Structure
Model parameterization:
The model is parameterized as a cell model, where the earth is divided into many cells. 
Vertical parameterization: 100km between each layer from surface to CMB with exception at 410 and 660km.
Horizontal parameterization: 65km*65km blocks on CMB and extended vertically to surface with exception at poles.



#### Things To Do
- [ ] Benchmark systematically and extensively test the algorithm with different parameter and starting model and keep track of how results changes
- [ ] Document workflow and efficiency
- [ ] Test the algorithm with data (regular phase) and explore a little bit
- [ ] Talk to Ed. about the results for possible ideas
- [ ] Produce necessary plots to evaluate the results
The following plots are proposed to make:
```
-	Coverage map of each layer -> paraview of the whole 3D coverage model
-	Raw raypath of all record in paraview
-	Travel time anomaly histogram after each iteration
-	Variance Reduction after each iteration
-	Travel time anomaly histogram after each layer update
-	Updated tomography model after each iteration
-	Residual tomography of each iteration (difference between current tomography and updated tomography in current iteration)
-	Sampling density map
-	Crossing map (based on azimuthal coverage of each cell)
-	Data quality map
```
