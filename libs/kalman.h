#ifndef _KALMAN
#define _KALMAN

typedef struct {
    // The predicted from gyro data
    float predicted_roll; 
    float predicted_pitch;
    // Original gyro predictions
    float gyro_roll;
    float gyro_pitch;
    // measured from Accelerometer
    float measured_roll;  
    float measured_pitch;
} KalmanState;

typedef struct {
    float P[2][2]; // Error covariance matrix
    float Q[2][2]; // Process noise variance for the accelerometer
    float R[2][2]; // Measurement noise variance
    float K[2][2]; // Kalman gain
   
} KalmanFilter;

void KalmanInit(KalmanState *state, KalmanFilter *filter);

void KalmanUpdate(KalmanState *state, KalmanFilter *filter, float newAngle[2], float newRate[3], float dt);

#endif