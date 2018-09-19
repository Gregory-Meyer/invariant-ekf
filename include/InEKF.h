#ifndef INEKF_H
#define INEKF_H 
#include <Eigen/Dense>
#include <Eigen/StdVector>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "RobotState.h"
#include "LieGroup.h"

namespace inekf {

typedef std::map<int,Eigen::Vector3d, std::less<int>, Eigen::aligned_allocator<std::pair<const int,Eigen::Vector3d>>> mapIntVector3d;
typedef std::vector<std::pair<int,Eigen::Vector3d>, Eigen::aligned_allocator<std::pair<int,Eigen::Vector3d>>> vectorPairIntVector3d;

class NoiseParams {

    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        NoiseParams();

        void setGyroscopeNoise(double std);
        void setGyroscopeNoise(const Eigen::Vector3d& std);
        void setGyroscopeNoise(const Eigen::Matrix3d& cov);

        void setAccelerometerNoise(double std);
        void setAccelerometerNoise(const Eigen::Vector3d& std);
        void setAccelerometerNoise(const Eigen::Matrix3d& cov);  

        void setGyroscopeBiasNoise(double std);
        void setGyroscopeBiasNoise(const Eigen::Vector3d& std);
        void setGyroscopeBiasNoise(const Eigen::Matrix3d& cov);

        void setAccelerometerBiasNoise(double std);
        void setAccelerometerBiasNoise(const Eigen::Vector3d& std);
        void setAccelerometerBiasNoise(const Eigen::Matrix3d& cov);  

        void setLandmarkNoise(double std);
        void setLandmarkNoise(const Eigen::Vector3d& std);
        void setLandmarkNoise(const Eigen::Matrix3d& cov);

        Eigen::Matrix3d getGyroscopeCov();
        Eigen::Matrix3d getAccelerometerCov();
        Eigen::Matrix3d getGyroscopeBiasCov();
        Eigen::Matrix3d getAccelerometerBiasCov();
        Eigen::Matrix3d getLandmarkCov();

        friend std::ostream& operator<<(std::ostream& os, const NoiseParams& p);  

    private:
        Eigen::Matrix3d Qg_;
        Eigen::Matrix3d Qa_;
        Eigen::Matrix3d Qbg_;
        Eigen::Matrix3d Qba_;
        Eigen::Matrix3d Ql_;
};




class Observation {

    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        Observation(Eigen::VectorXd& Y, Eigen::VectorXd& b, Eigen::MatrixXd& H, Eigen::MatrixXd& N, Eigen::MatrixXd& PI);
        bool empty();

        Eigen::VectorXd Y;
        Eigen::VectorXd b;
        Eigen::MatrixXd H;
        Eigen::MatrixXd N;
        Eigen::MatrixXd PI;

        friend std::ostream& operator<<(std::ostream& os, const Observation& o);  
};


class InEKF {
    
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        InEKF();
        InEKF(RobotState state);
        InEKF(RobotState state, const mapIntVector3d& prior_landmarks);

        RobotState getState();
        NoiseParams getNoiseParams();
        mapIntVector3d getPriorLandmarks();
        void setState(RobotState state);
        void setNoiseParams(NoiseParams params);
        void setPriorLandmarks(const mapIntVector3d& prior_landmarks);

        void Propagate(const Eigen::Matrix<double,6,1>& m, double dt);
        void Correct(const Observation& obs);
        void CorrectLandmarks(const vectorPairIntVector3d& measured_landmarks);

    private:
        RobotState state_;
        NoiseParams noise_params_;
        const Eigen::Vector3d g_ = (Eigen::VectorXd(3) << 0,0,-9.81).finished(); // Gravity
        mapIntVector3d prior_landmarks_;
        std::map<int,int> estimated_landmarks_;
};

} // end inekf namespace
#endif 
