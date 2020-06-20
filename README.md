# Q-tracer (work in progress..)
Path tracer based on Reinforcement Learning algorithms.

This path tracer allows to set different importance sampling approaches based on value-based Reinforcement Learning (Q-Learning) for physically-based path tracing. Its focus is on customization and experimentation rather than performance. 
The different approaches are:
- Cosine-weighted importance sampling.
- Next Event Estimation. Its result is used as a reference image to compare the various methods.
- An implementation of the paper [<i>Learning light transport the reinforced way</i>](https://arxiv.org/abs/1701.07403) [1] by Dahm and Keller. 
- An improvement of the previous approach. Specifically, a different action space definition and additional edits in Q-Learning improved the final results.
- Q-Learning based method to the popular [<i>Practical Path Guiding for Efficient Light-Transport Simulation</i>](https://tom94.net/data/courses/vorba19guiding/vorba19guiding-chapter10.pdf) approach by Müller et al.[2], which was adapted to be framed as a Markov Decision Process. 
- Based on the Q-Path Guiding method, the Q-Learning approach with no-delayed reward was implemented.

The various methods are then compared based on the SSIM index at fixed SPP.

The various path tracing strategies, as well as additional parameters, can be selected in `config.txt`.

# TO DO:
- [x] Add cosine-weighted and NEE
- [x] Add Q-Learning and improved action space
- [ ] Add Q-Path Guiding approach
- [ ] Add Q-learning, no-delayed reward
- [ ] Add description of various methods
- [ ] Add instructions
- [ ] Report results
- [ ] Add scenes and materials

[1] Dahm, Ken, and Alexander Keller. "Learning light transport the reinforced way." ACM SIGGRAPH 2017 Talks. 2017. 1-2.
[2] Müller, Thomas, Markus Gross, and Jan Novák. "Practical path guiding for efficient light‐transport simulation." Computer Graphics Forum. Vol. 36. No. 4. 2017.
