#pragma once

#include "tkFile/TkmFile.h"
#include "MeshParts.h"
#include "Skeleton.h"

namespace nsK2EngineLow {
	class IShaderResource;

	//モデルの上方向
	enum EnModelUpAxis {
		enModelUpAxisY,		//モデルの上方向がY軸。
		enModelUpAxisZ,		//モデルの上方向がZ軸。
	};

	/// <summary>
	/// モデルの初期化データ
	/// </summary>
	struct ModelInitData {
		const char* m_tkmFilePath = nullptr;							// tkmファイルパス。
		const char* m_vsEntryPointFunc = "VSMain";						// 頂点シェーダーのエントリーポイント。
		const char* m_vsSkinEntryPointFunc = "VSMain";					// スキンありマテリアル用の頂点シェーダーのエントリーポイント。
		const char* m_psEntryPointFunc = "PSMain";						// ピクセルシェーダーのエントリーポイント。
		const char* m_fxFilePath = nullptr;								// .fxファイルのファイルパス。
		void* m_expandConstantBuffer = nullptr;							// ユーザー拡張の定数バッファ。
		int m_expandConstantBufferSize = 0;								// ユーザー拡張の定数バッファのサイズ。
		std::array<IShaderResource*, MAX_MODEL_EXPAND_SRV> m_expandShaderResoruceView = { nullptr };	// ユーザー拡張のシェーダーリソース。
																										// t10レジスタから順番に割り当てられます。
		Skeleton* m_skeleton = nullptr;									// スケルトン。
		EnModelUpAxis m_modelUpAxis = enModelUpAxisZ;					// モデルの上方向。
		AlphaBlendMode m_alphaBlendMode = AlphaBlendMode_None;			// アルファブレンディングモード。
		bool m_isDepthWrite = true;										// 深度バッファに書き込む？
		bool m_isDepthTest = true;										// 深度テストを行う？
		D3D12_CULL_MODE m_cullMode = D3D12_CULL_MODE_BACK;				// カリングモード。
		std::array<DXGI_FORMAT, MAX_RENDERING_TARGET> m_colorBufferFormat = {
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			DXGI_FORMAT_UNKNOWN,
			DXGI_FORMAT_UNKNOWN,
			DXGI_FORMAT_UNKNOWN,
			DXGI_FORMAT_UNKNOWN,
			DXGI_FORMAT_UNKNOWN,
			DXGI_FORMAT_UNKNOWN,
		};	//レンダリングするカラーバッファのフォーマット。
	};
	/// <summary>
	/// マテリアルを再初期化するためのデータ。
	/// </summary>
	struct MaterialReInitData {
		std::array<IShaderResource*, MAX_MODEL_EXPAND_SRV> m_expandShaderResoruceView = { nullptr };
	};
	/// <summary>
	/// モデルクラス。
	/// </summary>
	class Model : public Noncopyable {

	public:

		/// <summary>
		/// tkmファイルから初期化。
		/// </summary>
		/// <param name="initData">初期化データ</param>
		void Init(const ModelInitData& initData);
		/// <summary>
		/// ワールド行列の更新。
		/// </summary>
		/// <param name="pos">座標</param>
		/// <param name="rot">回転</param>
		/// <param name="scale">拡大率</param>
		void UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
		{
			m_worldMatrix = CalcWorldMatrix(pos, rot, scale);
		}

		/// <summary>
		/// ワールド行列を計算
		/// </summary>
		/// <remark>
		/// Modelクラスの設定に基づいたワールド行列の計算が行われます。
		/// 計算されたワールド行列が戻り値として返されます。
		/// 本関数はUpdateWorldMatrixから呼ばれています。
		/// 本関数はワールド行列を計算して、戻すだけです。
		/// Model::m_worldMatrixが更新されるわけではないので、注意してください。
		/// 本クラスの設定に基づいて計算されたワールド行列が必要な場合に使用してください
		/// </remark>
		/// <param name="pos">座標</param>
		/// <param name="rot">回転</param>
		/// <param name="scale">拡大率</param>
		/// <returns>ワールド行列</returns>
		Matrix CalcWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale);
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="renderContext">レンダリングコンテキスト</param>
		/// <param name="numInstance">インスタンスの数</param>
		void Draw(
			RenderContext& renderContext,
			int numInstance = 1
		);
		/// <summary>
		/// 描画(カメラ指定版)
		/// </summary>
		/// <param name="renderContext">レンダリングコンテキスト</param>
		/// <param name="camera">カメラ</param>
		/// <param name="numInstance">インスタンスの数</param>
		void Draw(
			RenderContext& renderContext,
			Camera& camera,
			int numInstance = 1
		);
		/// <summary>
		/// 描画(カメラ行列指定版)
		/// </summary>
		/// <param name="renderContext">レンダリングコンテキスト</param>
		/// <param name="viewMatrix">ビュー行列</param>
		/// <param name="projMatrix">プロジェクション行列</param>
		/// <param name="numInstance">インスタンスの数</param>
		void Draw(
			RenderContext& renderContext,
			const Matrix& viewMatrix,
			const Matrix& projMatrix,
			int numInstance = 1
		);
		/// <summary>
		/// ワールド行列を取得。
		/// </summary>
		/// <returns></returns>
		const Matrix& GetWorldMatrix() const
		{
			return m_worldMatrix;
		}
		/// <summary>
		/// メッシュに対して問い合わせを行う。
		/// </summary>
		/// <param name="queryFunc">問い合わせ関数</param>
		void QueryMeshs(std::function<void(const SMesh& mesh)> queryFunc)
		{
			m_meshParts.QueryMeshs(queryFunc);
		}
		void QueryMeshAndDescriptorHeap(std::function<void(const SMesh& mesh, const DescriptorHeap& ds)> queryFunc)
		{
			m_meshParts.QueryMeshAndDescriptorHeap(queryFunc);
		}
		/// <summary>
		/// アルベドマップを変更。
		/// </summary>
		/// <remarks>
		/// この関数を呼び出すとディスクリプタヒープの再構築が行われるため、処理負荷がかかります。
		/// 毎フレーム呼び出す必要がない場合は呼び出さないようにしてください。
		/// </remarks>
		/// <param name="materialName">変更しいたマテリアルの名前</param>
		/// <param name="albedoMap">アルベドマップ</param>
		void ChangeAlbedoMap(const char* materialName, Texture& albedoMap);
		/// <summary>
		/// TKMファイルを取得。
		/// </summary>
		/// <returns></returns>
		const TkmFile& GetTkmFile() const
		{
			return *m_tkmFile;
		}
		/// <summary>
		/// 初期化されているか判定。
		/// </summary>
		/// <returns></returns>
		bool IsInited() const
		{
			return m_isInited;
		}
		/// <summary>
		/// マテリアルを再初期化。
		/// </summary>
		/// <remark>
		/// モデルに貼り付けるテクスチャを変更したい場合などに利用してください。
		/// </remark>
		/// <param name="reInitData">再初期化データ。</param>
		void ReInitMaterials(MaterialReInitData& reInitData);


		TkmFile* GetTkmFile()
		{
			return m_tkmFile;
		}

		bool m_isHit;

		//ベクトル内積
		double dot_product(const Vector3& vl, const Vector3 vr) {
			return vl.x * vr.x + vl.y * vr.y + vl.z * vr.z;
		}

		//三角形と点の当たり判定
		bool hittest_polygon_3d(Vector3 A, Vector3 B, Vector3 C, Vector3 P)
		{
			Vector3 AB, BP, BC, CP, CA, AP;

			//ベクトルの減算
			AB.Subtract(B, A);
			BP.Subtract(P, B);

			BC.Subtract(C, B);
			CP.Subtract(P, C);

			CA.Subtract(A, C);
			AP.Subtract(P, A);

			Vector3 c1, c2, c3;

			//外積を求める
			c1.Cross(AB, BP);
			c2.Cross(BC, CP);
			c3.Cross(CA, AP);

			//内積で順方向か逆方向か調べる
			double dot_12 = dot_product(c1, c2);
			double dot_13 = dot_product(c1, c3);

			if (dot_12 > 0 && dot_13 > 0) {

				int hit = 0;
				//三角形の内側に点がある
				return true;
			}

			int nohit = 0;
			//三角形の外側に点がある
			return false;
		}



		//線分ABと平面の交点を計算する
		bool IntersectPlaneAndLine(
			Vector3& out,	//戻り値　交点が見つかれば格納される 衝突点
			Vector2& uv,	//戻り値　交点が見つかれば格納される 衝突したポリゴンの３頂点のUV座標
			Vector3& A,		//線分始点
			Vector3& B,		//線分終点
			std::vector<nsK2EngineLow::TkmFile::VectorBuffer>& vectorBuffer
		) //平面
		{
			for (int i = 0;i < vectorBuffer.size();i++)
			{


				Matrix matrix;

				//ワールド行列を取得
				matrix = GetWorldMatrix();

				//頂点座標をワールド座標に変換
				matrix.Apply(vectorBuffer[i].buffer[0]);
				matrix.Apply(vectorBuffer[i].buffer[1]);
				matrix.Apply(vectorBuffer[i].buffer[2]);

				//法線をワールド座標に変換
				matrix.Apply(vectorBuffer[i].normal);


				//平面上の点P
				//点P１＝頂点１（Vector3）
				Vector3 P = Vector3(vectorBuffer[i].buffer[0]);

				//PA PBベクトル
				Vector3 PA = Vector3(P.x - A.x, P.y - A.y, P.z - A.z);
				Vector3 PB = Vector3(P.x - B.x, P.y - B.y, P.z - B.z);

				//PA PBそれぞれを平面法線と内積
				double dot_PA = PA.Dot(vectorBuffer[i].normal);
				double dot_PB = PB.Dot(vectorBuffer[i].normal);

				//これは線端が平面上にあった時の計算の誤差を吸収しています。調整して使ってください。
				if (abs(dot_PA) < 0.000001) { dot_PA = 0.0; }
				if (abs(dot_PB) < 0.000001) { dot_PB = 0.0; }

				//交差判定
				if (dot_PA == 0.0 && dot_PB == 0.0) {
					//両端が平面上にあり、交点を計算できない。
					continue;
				}
				if ((dot_PA >= 0.0 && dot_PB <= 0.0) ||
					(dot_PA <= 0.0 && dot_PB >= 0.0)) {
					//内積の片方がプラスで片方がマイナスなので、交差している

				}
				else {
					NOHit();
					//交差していない
					continue;
				}

				//以下、交点を求める 

				Vector3 AB = Vector3(B.x - A.x, B.y - A.y, B.z - A.z);

				//交点とAの距離 : 交点とBの距離 = dot_PA : dot_PB
				double hiritu = abs(dot_PA) / (abs(dot_PA) + abs(dot_PB));

				//衝突点の座標を求める。
				out.x = A.x + (AB.x * hiritu);
				out.y = A.y + (AB.y * hiritu);
				out.z = A.z + (AB.z * hiritu);


				if (hittest_polygon_3d(vectorBuffer[i].buffer[0], vectorBuffer[i].buffer[1], vectorBuffer[i].buffer[2], out) == false)
				{
					continue;
				}

				//
				auto v0v1 = vectorBuffer[i].buffer[1] - vectorBuffer[i].buffer[0];
				auto v0h = out - vectorBuffer[i].buffer[0];
				float z = dot_product(v0v1, v0h);
				z *= 0.5f;

				auto v1v2 = vectorBuffer[i].buffer[2] - vectorBuffer[i].buffer[1];
				auto v1h = out - vectorBuffer[i].buffer[1];
				float x = dot_product(v1v2, v1h);
				x *= 0.5f;

				auto v2v0 = vectorBuffer[i].buffer[0] - vectorBuffer[i].buffer[2];
				auto v2h = out - vectorBuffer[i].buffer[2];
				float y = dot_product(v2v0, v2h);
				y *= 0.5f;


				float xyz = x + y + z;
				float uvx, uvy, uvz;


				uvx = x / xyz;
				uvy = y / xyz;
				uvz = z / xyz;

				//合計が１になる
				float a = uvx + uvy + uvz;

				Vector2 Huv0;
				Huv0.x = vectorBuffer[i].uv[0].x * uvx;
				Huv0.y = vectorBuffer[i].uv[0].y * uvx;
				Vector2 Huv1;
				Huv1.x = vectorBuffer[i].uv[1].x * uvy;
				Huv1.y = vectorBuffer[i].uv[1].y * uvy;
				Vector2 Huv2;
				Huv2.x = vectorBuffer[i].uv[2].x * uvz;
				Huv2.y = vectorBuffer[i].uv[2].y * uvz;

				//衝突点のUV座標を求める。
				uv.x = Huv0.x + Huv1.x + Huv2.x;
				uv.y = Huv0.y + Huv1.y + Huv2.y;


				HitFlag();

				int ai = 0;
				return true;
			}

			return false;
		}


		void NOHit()
		{
			m_isHit = false;
		}

		void HitFlag()
		{
			m_isHit = true;
		}

		bool IsHitFlag()
		{
			return m_isHit;
		}




		/// <summary>
/// シャドウレシーバーのフラグを設定
/// </summary>
/// <param name="isFlag"></param>
		void SetShadowReciever(bool isFlag)
		{
			m_meshParts.SetShadowReciever(isFlag);
		}
		/// <summary>
		/// シャドウレシーバーかどうかを取得
		/// </summary>
		/// <returns></returns>
		bool GetShadowReciever()
		{
			return m_meshParts.GetShadowReciever();
		}
		/// <summary>
		/// シャドウキャスターのフラグを設定
		/// </summary>
		/// <param name="isFlag"></param>
		void SetShadowCaster(bool isFlag)
		{
			m_isShadowCaster = isFlag;
		}
		/// <summary>
		/// シャドウキャスターかどうかを取得
		/// </summary>
		/// <returns></returns>
		bool GetShadowCaster()
		{
			return m_isShadowCaster;
		}

	private:
		bool m_isInited = false;						//初期化されている？
		Matrix m_worldMatrix;							//ワールド行列。
		TkmFile* m_tkmFile;								//tkmファイル。
		Skeleton m_skeleton;							//スケルトン。
		MeshParts m_meshParts;							//メッシュパーツ。
		EnModelUpAxis m_modelUpAxis = enModelUpAxisY;	//モデルの上方向。

		bool m_isShadowCaster = true;

	};
}
