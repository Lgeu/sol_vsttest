// VST3 SDKのインクルードファイル
#include "pluginterfaces\base\funknown.h"

// VST3作成に必要なの名前空間を使用
namespace Steinberg {
namespace Vst {

// FUIDの生成
// プロジェクトごとに変更する
static const FUID ProcessorUID(0x1B2FFC0A, 0x107344C7, 0xB06F2F18, 0x1F3ECD98);
static const FUID ControllerUID(0xD8A37D3F, 0x1E40458B, 0x8FDEC44C, 0x5B84A9FE);

}
} // namespace SteinbergとVstの終わり