#pragma once
#include <core/core.hpp>
#include "State.h"

#define BIN (8 * 256)   // ֱ��ͼ����
#define SHIFT 5 //log2( 256/8 )Ϊ�ƶ�λ��

#define SIGMA2 0.02
#define ALPHA_COEFFICIENT 0.2 // Ŀ��ģ�͸���Ȩ��

class Tracker
{
public:
	explicit Tracker(unsigned char width, unsigned char height)
		: _width(width),
		  _height(height),
		  _curFrame(nullptr),
		  _particles(nullptr),
		  _DELTA_T(0.05),
		  _VELOCITY_DISTURB(40.0),
		  _SCALE_DISTURB(0.0),
		  _SCALE_CHANGE_D(0.001),
		  _nParticle(100),
		  _modelHist(nullptr),
		  _particleWeights(nullptr),
		  _nbin(0),
		  _piThreshold(0.9)
	{
	}

	int ParticleTracking(unsigned short* image, int width, int height, int& centerX, int& centerY, int& halfWidthOfTarget, int& halfHeightOfTarget, float& max_weight);

	int Initialize(int centerX, int centerY, int halfWidthOfTarget, int halfHeightOfTarget, unsigned short* imgData, int width, int height);

private:
	void ReSelect(SpaceState* state, float* weight, int nParticle);

	static void ImportanceSampling(float* wights, int* ResampleIndex, int nParticle);

	static void NormalizeCumulatedWeight(float* weight, float* cumulateWeight, int nParticle);

	static float rand01();

	float randGaussian(float u, float sigma) const;

	static int BinearySearch(float value, float* NCumuWeight, int N);

	void CalcuModelHistogram(int centerX, int centerY, int halfWidthOfTarget, int halfHeightofTarget, unsigned short* imgData, int width, int height, float* hist);

	void Propagate(SpaceState* state, int nParticle);

	void Observe(SpaceState* state, float* weight, int NParticle, unsigned short* imgData, int width, int height);

	float CalcuBhattacharyya(float* histA, float* histB) const;

	float CalcuWeightedPi(float rho) const;

	void Estimation(SpaceState* particles, float* weights, int NParticle, SpaceState& EstState);

	void ModelUpdate(SpaceState EstState, float* TargetHist, int bins, float PiT, unsigned short* imgData, int width, int height);

private:
	unsigned char _width; // Frame size : width
	unsigned char _height; // Frame size : height

	unsigned short* _curFrame;

	SpaceState* _particles; // ״̬����

	cv::Mat _curFrameMat;
	cv::Mat _trackingImg;

	float _DELTA_T;          // ֡Ƶ������Ϊ30��25��15��10��
	float _VELOCITY_DISTURB; // �ٶ��Ŷ���ֵ
	float _SCALE_DISTURB;    // ������Ŷ�����
	float _SCALE_CHANGE_D;   // �߶ȱ任�ٶ��Ŷ�����

	int _nParticle;    // ���Ӹ���
	float* _modelHist; // ģ��ֱ��ͼ
	float* _particleWeights; // ÿ�����ӵ�Ȩ��
	int _nbin;          // ֱ��ͼ����
	float _piThreshold; // Ȩ����ֵ
};
