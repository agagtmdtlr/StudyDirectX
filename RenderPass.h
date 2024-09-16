#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include <unordered_map>
#include <bitset>
#include <d3dcommon.h>

enum class RenderStage : UINT
{
	VS = 0,
	PS = VS + 1,
	CS = PS + 1,
	NUM = CS + 1,
	Begin = 0
};


#define StageCount (static_cast<size_t>(RenderStage::NUM))

using BindStageMask = std::bitset<static_cast<size_t>(RenderStage::NUM)>;

struct RenderPassState
{
	void SetFlag(RenderStage flag, bool val)
	{
		mask[static_cast<size_t>(flag)] = val;
	}

	bool CheckMask(RenderStage flag) const
	{
		return mask[static_cast<size_t>(flag)];
	}

	glm::vec4 clearColor;
	BindStageMask mask;
};

struct RenderPass
{
	using BindDescList = std::array<D3D11_SHADER_INPUT_BIND_DESC, StageCount>;


	D3D11_VIEWPORT viewport;

	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11GeometryShader> geometryShader;
	ComPtr<ID3D11DomainShader> domainShader;
	ComPtr<ID3D11PixelShader> pixelShader;

	ComPtr<ID3D11ComputeShader> computeShader;

	ComPtr<ID3D11InputLayout> layout;
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;

	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;


	ComPtr<ID3D11RenderTargetView> rtv;
	ComPtr<ID3D11DepthStencilView> dsv;

	std::array<ComPtr<ID3DBlob>, StageCount> blobs;

	RenderPassState state;

	D3D11_SHADER_DESC shaderDesc;
	std::unordered_map<std::string, BindDescList> bindResources;

	RenderPass(std::wstring shaderName , RenderPassState state);

	void Initialize(std::wstring shaderName, RenderPassState state);

	void DrawIndexed(UINT indexCount, UINT StartIndexLocation, INT BseVertexLocation);

	void BeginDraw();
	void DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation);
	void EndDraw();

private:
	ID3DBlob* GetBlob(RenderStage stage);

	/*
	set sample
	set shader resource ( csv , srv, uav
	*/
	

};

