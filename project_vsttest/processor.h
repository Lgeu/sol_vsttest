#pragma once

// VST3 SDK�̃C���N���[�h�t�@�C��
#include "public.sdk/source/vst/vstaudioeffect.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h" // �C�x���g�o�X���p���ɕK�v

#include "myvst3define.h"

#include <random>

// VST3�쐬�ɕK�v�Ȃ̖��O��Ԃ��g�p
namespace Steinberg {
namespace Vst {

// ===================================================================================
// �����M������������Processor�N���X
// ===================================================================================
class MyVSTProcessor : public AudioEffect {
protected:
	/*
	ParamValue volume, depth, freq;
	int32 type;
	*/
	std::vector<int32> noteList;
	ParamValue volume, detune1, gain1;
	int32 octave1;
	std::vector<std::vector<float>> theta1;  // �e�g�̈ʑ��C[0,1)
	float thetaDebug;
	int debugFlag;

	// ���������֘A
	std::random_device rnd;
	std::mt19937 mt;
	std::uniform_real_distribution<float> randPhase;

public:
	// �R���X�g���N�^
	MyVSTProcessor();

	// �N���X������������֐�(�K�{)
	tresult PLUGIN_API initialize(FUnknown* context);

	// �o�X�\����ݒ肷��֐�
	tresult PLUGIN_API setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts);

	// �����M������������֐�(�K�{)
	tresult PLUGIN_API process(ProcessData& data);

	// MIDI�m�[�g�I���C�x���g�AMIDI�m�[�g�I�t�C�x���g���󂯎�����ꍇ�ɏ�������֐�
	virtual void onNoteOn(int channel, int note, float velocity);
	virtual void onNoteOff(int channel, int note, float velocity);

	// ����VST Processor�N���X�̃C���X�^���X���쐬���邽�߂̊֐�(�K�{)
	static FUnknown* createInstance(void*) { return (IAudioProcessor*)new MyVSTProcessor(); }
};


}
} // namespace Steinberg��Vst�̏I���