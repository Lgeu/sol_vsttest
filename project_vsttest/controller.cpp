// ����VST�p�̃C���N���[�h�t�@�C��
#include "myvst3define.h"
#include "myvst3fuid.h"
#include "controller.h"


// VST3�쐬�ɕK�v�Ȃ̖��O��Ԃ��g�p
namespace Steinberg {
namespace Vst {


// �N���X������������֐�(�K�{)
tresult PLUGIN_API MyVSTController::initialize(FUnknown* context)
{
	// �܂��p�����N���X�̏����������{
	tresult result = EditController::initialize(context);
	if (result == kResultTrue) {
		// �p�����[�^�[��ǉ�
		parameters.addParameter(STR16("param1"), STR16("..."), 0, 1, ParameterInfo::kCanAutomate, PARAM1_TAG);

		// �ȉ��ŗL�̏����������{�B

		// ����͉������Ȃ�
	}

	// �����������������kResultTrue��Ԃ��B
	result = kResultTrue;
	return result;
}


}
} // namespace Steinberg��Vst�̏I���