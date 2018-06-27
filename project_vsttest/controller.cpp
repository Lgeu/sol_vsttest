// ����VST�p�̃C���N���[�h�t�@�C��
#include "myvst3define.h"
#include "myvst3fuid.h"
#include "controller.h"


// VST3�쐬�ɕK�v�Ȃ̖��O��Ԃ��g�p
namespace Steinberg {
namespace Vst {


// �N���X������������֐�(�K�{)
tresult PLUGIN_API MyVSTController::initialize(FUnknown* context){
	// �܂��p�����N���X�̏����������{
	tresult result = EditController::initialize(context);
	if (result == kResultTrue) {
		/*
		// �W���i�͈�0�`1�j�̃p�����[�^�[���R���g���[���[�ɒǉ�
		// ���O�C�P�ʁi�ȗ��j�C�i�K�C�W���̒l�C�t���O�C�^�O�CUnitID
		parameters.addParameter(STR16("Depth"), STR16("..."), 0, 1, ParameterInfo::kCanAutomate, PARAM_DEPTH_TAG);

		// �C�ӂ͈̔͂��w��ł���p�����[�^�̒ǉ��iVST�ɑ�����l��0�`1�ɐ��K�������j
		// �͈̓p�����[�^�[���쐬
		// ���O�C�^�O�C�P�ʁC�ŏ��l�i0.0�j�C�ő�l�i1.0�j�C�����l�i0.0�j�C�i�K���i0�j�C�t���O�i�ȗ��j�CUnitId�i�ȗ��j
		RangeParameter* param1 = new RangeParameter(STR16("Speed"), PARAM_SPEED_TAG, STR16("Hz"), 0.5f, 30.0f, 5.0f);
		param1->setPrecision(2);  // �����扽�ʂ܂ŕ\�����邩
		parameters.addParameter(param1);  // �͈̓p�����[�^�[���R���g���[���[�ɒǉ�

		// �����񃊃X�g�p�����[�^�[���쐬�A�ǉ�
		StringListParameter* param2 = new StringListParameter(STR16("Type"), PARAM_TYPE_TAG);
		param2->appendString(STR16("Volume"));  // ���X�g���ڂ�ǉ�
		param2->appendString(STR16("Tremolo"));
		param2->appendString(STR16("Panning"));
		// �����񃊃X�g�p�����[�^�[���R���g���[���[�ɒǉ�
		parameters.addParameter(param2);
		*/

		// �I�V���[�^1
		// Octave�p�����[�^��ǉ�
		RangeParameter* param101 = new RangeParameter(STR16("Osc1 Octave"), PARAM_OSC1_OCTAVE_TAG, STR16(""), -3.0f, 3.0f, 0.0f, 7);
		param101->setPrecision(0);
		parameters.addParameter(param101);
		// Detune�p�����[�^��ǉ�
		RangeParameter* param102 = new RangeParameter(STR16("Osc1 Detune"), PARAM_OSC1_DETUNE_TAG, STR16("cent"), 0.0f, 100.0f, 0.0f);
		param102->setPrecision(1);
		parameters.addParameter(param102);
		// Gain�p�����[�^��ǉ�
		RangeParameter* param103= new RangeParameter(STR16("Osc1 Gain"), PARAM_OSC1_GAIN_TAG, STR16(""), 0.0f, 1.0f, 0.0f);
		param103->setPrecision(2);
		parameters.addParameter(param103);

		// Volume�p�����[�^��ǉ�
		RangeParameter* param1001= new RangeParameter(STR16("Volume"), PARAM_VOLUME_TAG, STR16(""), 0.0f, 1.0f, 0.0f);
		param1001->setPrecision(2);
		parameters.addParameter(param1001);


		// �ȉ��ŗL�̏����������{

		// ����͉������Ȃ�
	}

	// �����������������kResultTrue��Ԃ��B
	result = kResultTrue;
	return result;
}


}
} // namespace Steinberg��Vst�̏I���