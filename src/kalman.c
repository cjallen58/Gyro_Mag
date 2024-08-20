#include "../libs/kalman.h"

// Kalman filter initialization
void KalmanInit(KalmanState *state, KalmanFilter *filter) {  
        //initial state
        state->predicted_roll = 0.0f;
        state->predicted_pitch = 0.0f;
        state->measured_roll = 0.0f;
        state->measured_pitch = 0.0f;

        //initial error covariance matrix
        filter->P[0][0] = 0.1f;
        filter->P[0][1] = 0.0f;
        filter->P[1][0] = 0.0f;
        filter->P[1][1] = 0.1f;

        //initial process noise covariance matrix
        filter->Q[0][0] = 0.001f;
        filter->Q[0][1] = 0.000f;
        filter->Q[1][0] = 0.000f;
        filter->Q[1][1] = 0.001f;

        //initial Measurement Noise Covariance Matrix
        filter->R[0][0] = 0.05f;
        filter->R[0][1] = 0.00f;
        filter->R[1][0] = 0.00f;
        filter->R[1][1] = 0.05f;
}

void KalmanUpdate(KalmanState *state, KalmanFilter *filter, float newAngle[2], float newRate[3], float dt) {
    // Step 1 Predict
    // State prediction with gyro measurements
    state->predicted_roll +=  newRate[0] * dt;
    state->predicted_pitch += newRate[1] * dt;

    state->gyro_roll = state->predicted_roll;
    state->gyro_pitch = state->predicted_pitch;
    
    // Error Covariance predicition 
    filter->P[0][0] += filter->Q[0][0];
    filter->P[1][1] += filter->Q[1][1];

    state->measured_roll = newAngle[0];
    state->measured_pitch = newAngle[1];


    // Step 2 Update
    // Gain update
    filter->K[0][0] = filter->P[0][0] / (filter->P[0][0] + filter->R[0][0]);
    filter->K[1][1] = filter->P[1][1] / (filter->P[1][1] + filter->R[1][1]);

    // state update with gain and measured roll
    state->predicted_roll += filter->K[0][0] * (state->measured_roll - state->predicted_roll);
    state->predicted_pitch += filter->K[1][1] * (state->measured_pitch - state->predicted_pitch);

    filter->P[0][0] = (1 - filter->K[0][0]) * filter->P[0][0];
    filter->P[1][1] = (1 - filter->K[1][1]) * filter->P[1][1];
}
