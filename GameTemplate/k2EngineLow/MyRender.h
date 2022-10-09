#pragma once

namespace nsK2EngineLow {
	// シーンのディレクションライトの最大数
	const int MAX_DIRECTIONAL_LIGHT = 4;
	// シーンのポイントライトの最大数。
	const int MAX_POINT_LIGHT = 256;
	// シーンのスポットライトの最大数。
	const int MAX_SPOT_LIGHT = 256;

	// シャドウマップ
	enum {
		SHADOW_MAP_AREA_NEAR,		//近影範囲のシャドウマップ。
		SHADOW_MAP_AREA_MIDDLE,		//中影範囲のシャドウマップ。
		SHADOW_MAP_AREA_FAR,		//遠影範囲のシャドウマップ。
		NUM_SHADOW_MAP
	};
}