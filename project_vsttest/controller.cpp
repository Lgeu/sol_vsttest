// 自作VST用のインクルードファイル
#include "myvst3define.h"
#include "myvst3fuid.h"
#include "controller.h"


// VST3作成に必要なの名前空間を使用
namespace Steinberg {
namespace Vst {


// クラスを初期化する関数(必須)
tresult PLUGIN_API MyVSTController::initialize(FUnknown* context){
	// まず継承元クラスの初期化を実施
	tresult result = EditController::initialize(context);
	if (result == kResultTrue) {
		/*
		// 標準（範囲0～1）のパラメーターをコントローラーに追加
		// 名前，単位（省略可），段階，標準の値，フラグ，タグ，UnitID
		parameters.addParameter(STR16("Depth"), STR16("..."), 0, 1, ParameterInfo::kCanAutomate, PARAM_DEPTH_TAG);

		// 任意の範囲を指定できるパラメータの追加（VSTに送られる値は0～1に正規化される）
		// 範囲パラメーターを作成
		// 名前，タグ，単位，最小値（0.0），最大値（1.0），初期値（0.0），段階数（0），フラグ（省略可），UnitId（省略可）
		RangeParameter* param1 = new RangeParameter(STR16("Speed"), PARAM_SPEED_TAG, STR16("Hz"), 0.5f, 30.0f, 5.0f);
		param1->setPrecision(2);  // 小数第何位まで表示するか
		parameters.addParameter(param1);  // 範囲パラメーターをコントローラーに追加

		// 文字列リストパラメーターを作成、追加
		StringListParameter* param2 = new StringListParameter(STR16("Type"), PARAM_TYPE_TAG);
		param2->appendString(STR16("Volume"));  // リスト項目を追加
		param2->appendString(STR16("Tremolo"));
		param2->appendString(STR16("Panning"));
		// 文字列リストパラメーターをコントローラーに追加
		parameters.addParameter(param2);
		*/

		// オシレータ1
		// Octaveパラメータを追加
		RangeParameter* param101 = new RangeParameter(STR16("Osc1 Octave"), PARAM_OSC1_OCTAVE_TAG, STR16(""), -3.0f, 3.0f, 0.0f, 7);
		param101->setPrecision(0);
		parameters.addParameter(param101);
		// Detuneパラメータを追加
		RangeParameter* param102 = new RangeParameter(STR16("Osc1 Detune"), PARAM_OSC1_DETUNE_TAG, STR16("cent"), 0.0f, 100.0f, 0.0f);
		param102->setPrecision(1);
		parameters.addParameter(param102);
		// Gainパラメータを追加
		RangeParameter* param103= new RangeParameter(STR16("Osc1 Gain"), PARAM_OSC1_GAIN_TAG, STR16(""), 0.0f, 1.0f, 0.0f);
		param103->setPrecision(2);
		parameters.addParameter(param103);

		// Volumeパラメータを追加
		RangeParameter* param1001= new RangeParameter(STR16("Volume"), PARAM_VOLUME_TAG, STR16(""), 0.0f, 1.0f, 0.0f);
		param1001->setPrecision(2);
		parameters.addParameter(param1001);


		// 以下固有の初期化を実施

		// 今回は何もしない
	}

	// 初期化が成功すればkResultTrueを返す。
	result = kResultTrue;
	return result;
}


}
} // namespace SteinbergとVstの終わり