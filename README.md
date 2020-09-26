# Q-tracer (work in progress..)
Path tracer based on various Reinforcement Learning algorithms.

This path tracer allows to set different importance sampling approaches based on value-based Reinforcement Learning (Q-Learning) for physically-based path tracing. Its focus is on customization and experimentation. The path tracer supports multi-threading with the [<i>OpenMP library</i>](https://www.openmp.org/).
The different approaches are:
- Cosine-weighted importance sampling.
- Next Event Estimation. This method is used as a reference image to compare the various methods.
- An implementation of the paper [<i>Learning light transport the reinforced way</i>](https://arxiv.org/abs/1701.07403) [1] by Dahm and Keller. 
- An improvement of the previous approach. Specifically, a different action space definition and additional edits in Q-Learning improved the final results.
- Q-Learning based method applied to the popular [<i>Practical Path Guiding for Efficient Light-Transport Simulation</i>](https://tom94.net/data/courses/vorba19guiding/vorba19guiding-chapter10.pdf) approach by Müller et al.[2], which was adapted to be framed as a Markov Decision Process. 
- A blended approach that combines the Q-Path Guiding method and Q-Learning approach. In this algorithm, the policy is only updated for the whole trace when a positive reward is received. 

The various methods are then compared based on the SSIM index at fixed SPP.

The various path tracing strategies, as well as additional parameters, can be selected in `misc\config.txt`.

## Run
Before compiling `smallpt.cpp`, you need to set the rendering parameters in `misc\config.txt`. <br>
The configuration file is structured as follows:
- `path_tracing_mode` : {0, 1, 2, 3, 4} <br>
    This parameter defines the scattering strategy. <br>
    0: Cosine-weighted importance sampling <br>
    1: Next Event Estimation <br>
    2: Q-Learning <br>
    3: Q-Path Guiding <br>
    4: QUpdate-PG <br>
    Only the number related to the chosen method needs to be written (example: `path_tracing_mode : 2`)
- `experiment_name` : name of the current experiment (example: `experiment_name: Experiment1`)
- `action_space_mode` : {0, 1} <br>
    This parameter defines the subdivision of the action space. <br>
    0: The action patches on the virtual hemisphere are equally-sized (Dahm and Keller's approach) <br>
    1: The action patches on the virtual hemisphere are not equally-sized (improved action space) 
- `scene` : {1,2,3} <br>
    1: Box scene <br>
    2: Sunrise scene <br>
    3: Door scene
- `w_train` : width of the image during training (example: `w_train: 128`)
- `h_train` : height of the image during training (example: `h_train: 128`) 
- `spp_train` : number of samples per pixel used during the training phase, only used if the `path_tracing_mode` supports training (example: `spp_train: 8`)
- `w_test` : width of the image during the final rendering (example: `w_test: 256`)
- `h_test` : height of the image during the final rendering (example: `h_test: 256`)
- `spp_test` : number of samples per pixel during the final rendering. If the `path_tracing_mode` selected does not support training, the renderer uses `spp_test` as number of samples per pixel (example: `spp_test: 128`)
- `training` : {0, 1} <br>
    Training activated (0) or deactivated (1). If the parameter is 0, the weights used for the final rendering are retrieved from the `weight_filename` parameter (if the `path_tracing_method` supports training).
- `test` : {0, 1} <br>
    Final rendering activated (0) or deactivated (1). The methods that are not based on Reinforcement Learning do not support training and require this value to be set to 1.  
- `weight_filename` : name of the file to read or store the weights of the RL policy (example: weights-QUpdate-eps05)



## TO DO:
- [x] Add cosine-weighted and NEE
- [x] Add Q-Learning and improved action space
- [x] Add Q-Path Guiding approach
- [x] Add Q-learning, no-delayed reward
- [x] Add multi-threading support
- [ ] Add support to load custom .obj files
- [ ] Add description of Reinforcement Learning methods
- [x] Add instructions
- [ ] Report results
- [ ] Add scenes and materials

## Literature
[1] Dahm, Ken, and Alexander Keller. "Learning light transport the reinforced way." ACM SIGGRAPH 2017 Talks. 2017. 1-2. <br>
[2] Müller, Thomas, Markus Gross, and Jan Novák. "Practical path guiding for efficient light‐transport simulation." Computer Graphics Forum. Vol. 36. No. 4. 2017.
