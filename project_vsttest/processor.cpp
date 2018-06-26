// VST3 SDK�̃C���N���[�h�t�@�C��
#include "public.sdk/source/main/pluginfactoryvst3.h"

// ����VST�p�̃C���N���[�h�t�@�C��
#include "myvst3define.h"
#include "myvst3fuid.h"
#include "processor.h"


// VST3�쐬�ɕK�v�Ȃ̖��O��Ԃ��g�p
namespace Steinberg {
namespace Vst {

// =================================================================================
// �R���X�g���N�^
// =================================================================================
MyVSTProcessor::MyVSTProcessor()
{
	// �R���g���[���[��FUID��ݒ肷��
	setControllerClass(ControllerUID);
}

// ===================================================================================
// �N���X������������֐�
// ===================================================================================
tresult PLUGIN_API MyVSTProcessor::initialize(FUnknown* context) {
	// �܂��p�����N���X�̏����������{
	tresult result = AudioEffect::initialize(context);
	if (result == kResultTrue) {
		// ���͂Əo�͂�ݒ�
		addAudioInput(STR16("AudioInput"), SpeakerArr::kStereo);
		addAudioOutput(STR16("AudioOutput"), SpeakerArr::kStereo);

		// �ȉ��ŗL�̏����������{�B

		volume = 1.0f;
	}


	// �����������������kResultTrue��Ԃ��B
	return result;
}

tresult PLUGIN_API MyVSTProcessor::setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts) {
	// input��output�̃o�X��1���ŁA����input��output�̍\�����X�e���I�̏ꍇ
	if (numIns == 1 && numOuts == 1 && inputs[0] == SpeakerArr::kStereo && outputs[0] == SpeakerArr::kStereo) {
		return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
	}

	// �Ή����Ă��Ȃ��o�X�\���̏ꍇ�AkResultFalse��Ԃ��B
	return kResultFalse;
}

// ===================================================================================
// �����M������������֐�
// ===================================================================================
tresult PLUGIN_API MyVSTProcessor::process(ProcessData& data){
	// �p�����[�^�[�ύX�̏���
	// �^����ꂽ�p�����[�^�[������Ƃ��Adata��inputParameterChanges��
	// IParameterChanges�N���X�ւ̃|�C���^�̃A�h���X������
	if (data.inputParameterChanges != NULL) {
		// �^����ꂽ�p�����[�^�[�̐����擾
		int32 paramChangeCount = data.inputParameterChanges->getParameterCount();

		// �^����ꂽ�p�����[�^�[���A�������J��Ԃ��B
		for (int32 i = 0; i < paramChangeCount; i++) {
			// �p�����[�^�[�ύX�̃L���[���擾
			// (��������T���v�����ɕ����̃p�����[�^�[�ύX��񂪂���\�������邽�߁A
			// �L���[�Ƃ����`�ɂȂ��Ă���B)
			IParamValueQueue* queue = data.inputParameterChanges->getParameterData(i);
			if (queue != NULL) {
				// �ǂ̃p�����[�^�[���ύX���ꂽ���m�邽�߁A�p�����[�^�[tag���擾
				int32 tag = queue->getParameterId();

				// �ύX���ꂽ�񐔂��擾
				int32 valueChangeCount = queue->getPointCount();
				ParamValue value;
				int32 sampleOffset;

				// �Ō�ɕύX���ꂽ�l���擾
				if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
					// tag�ɉ��������������{
					switch (tag) {
					case PARAM1_TAG:
						// volume��ύX����B
						volume = value;
						break;
					}
				}
			}
		}
	}

	// ���́E�o�̓o�b�t�@�̃|�C���^���킩��₷���ϐ��Ɋi�[
	// inputs[]�Aoutputs[]��AudioBus�̐���������(addAudioInput()�AaddAudioOutput()�Œǉ�����������)
	// �����AudioBus��1�����Ȃ̂� 0 �݂̂ƂȂ�
	// channelBuffers32��32bit���������_�^�̃o�b�t�@�ŉ����M���̃`�����l����������
	// ���m����(kMono)�Ȃ� 0 �݂̂ŁA�X�e���I(kStereo)�Ȃ� 0(Left) �� 1(Right) �ƂȂ�
	Sample32* inL = data.inputs[0].channelBuffers32[0];
	Sample32* inR = data.inputs[0].channelBuffers32[1];
	Sample32* outL = data.outputs[0].channelBuffers32[0];
	Sample32* outR = data.outputs[0].channelBuffers32[1];

	// numSamples�Ŏ������T���v�����A��������������
	for (int32 i = 0; i < data.numSamples; i++) {
		outL[i] = volume * inL[i];
		outR[i] = volume * inR[i];
	}

	// ���Ȃ����kResultTrue��Ԃ�(�����炭�K��kResultTrue��Ԃ�)
	return kResultTrue;
}

}
} // namespace Steinberg��Vst�̏I���