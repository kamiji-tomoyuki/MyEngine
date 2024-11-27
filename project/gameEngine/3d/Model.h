#pragma once
#include <d3d12.h>
#include <map>
#include <string>
#include <vector>
#include <wrl.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

class ModelCommon;

// 3Dモデル
class Model
{
private:
	// ===== 構造体 =====
	// --- 頂点データ ---
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	// --- 色データ ---
	struct Color {
		float r, g, b;
	};
	// --- マテリアル ---
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};
	// --- マテリアルデータ ---
	struct MaterialData {
		std::string name;
		float Ns;
		Color Ka; // 環境光色
		Color Kd; // 拡散反射色
		Color Ks; // 鏡面反射光
		float Ni;
		float d;
		uint32_t illum;
		std::string textureFilePath;
		uint32_t textureIndex = 0;
	};
	// --- 座標変換 ---
	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};
	// --- 平行光源 ---
	struct DirectionalLight {
		Vector4 color;     // ライトの色
		Vector3 direction; // ライトの向き
		float intensity;   // 輝度
	};
	// --- Transform ---
	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};
	// --- Node情報 ---
	struct Node {
		Matrix4x4 localMatrix;
		std::string name;
		std::vector<Node> children;
	};
	// --- モデルデータ ---
	struct ModelData {
		std::vector<VertexData> vertices;
		MaterialData material;
		Node rootNode;
		bool isAnimation;
	};
	// --- キーフレーム ---
	template <typename tValue>
	struct Keyframe {
		float time;		// キーフレームの時刻
		tValue value;	// キーフレームの値
	};
	using KeyframeVector3 = Keyframe<Vector3>;
	using KeyframeQuaternion = Keyframe<Quaternion>;
	// --- Nodeのアニメーション ---
	template <typename tValue>
	struct AnimationCurve {
		std::vector<Keyframe<tValue>> keyframes;
	};
	struct NodeAnimation
	{
		std::vector<KeyframeVector3> translate;
		std::vector<KeyframeQuaternion> rotate;
		std::vector<KeyframeVector3> scale;
	};
	// --- アニメーション ---
	struct Animation
	{
		float duration;	//アニメーション全体の尺
		// NodeAnimationの集合
		std::map<std::string, NodeAnimation> nodeAnimations;
	};

public:
	// 初期化
	void Initialize(ModelCommon* modelCommon, const std::string& directorypath, const std::string& filename);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

public:
	ModelData GetModelData() { return modelData_; }

private:
	//Data書き込み
	void VertexResource();
	void MaterialResource();

	// .mtlファイルの読み取り
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	//.objファイルの読み取り
	static ModelData LoadModelFile(const std::string& directoryPath, const std::string& filename);
	// Animationの解析
	Animation LoadAnimationFile(const std::string& directoryPath, const std::string& filename);
	
	// Node情報の読み取り
	static Node ReadNode(aiNode* node);
	// animationの時刻を取得
	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);

private:
	// --- ModelCommon ---
	ModelCommon* modelCommon_;

	// --- Objファイル ---
	ModelData modelData_;
	// アニメーションタイマー
	float animationTime = 0.0f;

	// --- バッファリソース ---
	// VertexResource
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	// VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	// VertexResourceにデータを書き込むためのポインタ
	VertexData* vertexData = nullptr;

	// --- マテリアル ---
	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	// マテリアルリソースにデータを書き込むためのポインタ
	Material* materialData = nullptr;
	
	// --- Transform ---
	Transform transform;

	// --- アニメーション ---
	Animation animation;

};

