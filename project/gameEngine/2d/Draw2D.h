#pragma once
#include "DirectXCommon.h"
#include <list>
#include <vector>

#include "Vector4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4x4.h"

using namespace std;

// ComPtrのエイリアス
template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

// ラインの描画
class Draw2D
{
#pragma region シングルトンインスタンス
private:
	static Draw2D* instance;

	Draw2D() = default;
	~Draw2D() = default;
	Draw2D(const Draw2D&) = delete;
	Draw2D& operator = (const Draw2D&) = delete;

public:
	// シングルトンインスタンスの取得
	static Draw2D* GetInstance();
	// 終了
	void Finalize();
#pragma endregion シングルトンインスタンス

public: 
	// ===== 構造体 =====
	struct VertexData
	{
		Vector2 position;
		Vector4 color;
	};

	// 座標変換行列データ
	struct TransformationMatrix
	{
		Matrix4x4 WVP;
	};

	// 三角形構造体
	struct TriangleData
	{
		VertexData* vertexData;
		// 頂点バッファ
		ComPtr<ID3D12Resource> vertexBuffer;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	};

	// 矩形構造体
	struct BoxData
	{
		VertexData* vertexData;
		uint32_t* indexData;
		// 頂点バッファ
		ComPtr<ID3D12Resource> vertexBuffer;
		// インデックスバッファ
		ComPtr<ID3D12Resource> indexBuffer;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		// インデックスバッファビュー
		D3D12_INDEX_BUFFER_VIEW indexBufferView;
	};

	// 線分構造体
	struct LineData
	{
		VertexData* vertexData;
		// 頂点バッファ
		ComPtr<ID3D12Resource> vertexBuffer;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	};

	struct Sphere {
		Vector3 center;
		float radius;
	};

public: // メンバ関数
	// 初期化
	void Initialize(DirectXCommon* dxCommon_);

	// 三角形の描画
	void DrawTriangle(const Vector2& pos1, const Vector2& pos2, const Vector2& pos3, const Vector4& color);

	// 矩形の描画
	void DrawBox(const Vector2& pos, const Vector2& size, const Vector4& color);
	void DrawBox(const Vector2& pos, const Vector2& size, const float angle, const Vector4& color);

	// 線の描画
	void DrawLine(const Vector2& start, const Vector2& end, const Vector4& color);
	void DrawLine(const Vector2& start, const Vector2& end, const Vector4& color, const Matrix4x4& viewProjectionMatrix);

	// 球の描画
	void DrawSphere(const Vector3& center, const float radius, const Vector4& color, const Matrix4x4& viewProjectionMatrix);

	// リセット
	void Reset();

public:
	// デバッグ用ビューマトリックスの取得
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }

	// プロジェクションマトリックスの設定
	void SetProjectionMatrix(const Matrix4x4& projectionMatrix) { projectionMatrix_ = projectionMatrix; }

private:
	// ルートシグネチャの作成
	void CreateRootSignature(ComPtr<ID3D12RootSignature>& rootSignature);

	// パイプラインステートの生成
	void CreatePSO(D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveTopologyType, ComPtr<ID3D12PipelineState>& pipelineState, ComPtr<ID3D12RootSignature>& rootSignature);

	// 三角形の頂点データの生成
	void CreateTriangleVertexData(TriangleData* triangleData);

	// 矩形の頂点データの生成
	void CreateBoxVertexData(BoxData* boxData);

	// 線の頂点データの生成
	void CreateLineVertexData(LineData* lineData);

	// 座標変換行列データの生成
	void CreateTransformMatData();

	// 球の頂点位置の計算
	void CalcSphereVertexData();

private:
	DirectXCommon* dxCommon_;

	const uint32_t kTrriangleMaxCount = 30096;
	const uint32_t kVertexCountTrriangle = 3;

	const uint32_t kBoxMaxCount = 30096;
	const uint32_t kVertexCountBox = 4;
	const uint32_t kIndexCountBox = 6;

	const uint32_t kLineMaxCount = 100000;
	const uint32_t kVertexCountLine = 2;

	// 三角形のインデックス
	uint32_t triangleIndex_ = 0;

	// 矩形のインデックス
	uint32_t boxIndexIndex_ = 0;
	uint32_t boxVertexIndex_ = 0;

	// 線のインデックス
	uint32_t lineIndex_ = 0;

	// マトリックス
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewPortMatrix_;

	Matrix4x4 debugViewMatrix_;
	Matrix4x4 debugProjectionMatrix_;

	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> triangleRootSignature_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> lineRootSignature_;

	// パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> trianglePipelineState_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> linePipelineState_;

	// 座標変換行列バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixBuffer_;

	// 座標変換行列データ
	TransformationMatrix* transformationMatrixData_;

	// 三角形データ
	TriangleData* triangleData_;

	// 矩形データ
	BoxData* boxData_;

	// 線データ
	LineData* lineData_;

	// 球のデータ
	std::vector<Vector3> spheres_;
};