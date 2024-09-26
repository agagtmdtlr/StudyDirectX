#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include <unordered_map>
#include <bitset>
#include <d3dcommon.h>
#include <array>
#include <d3dcompiler.h>

#include "PrimitiveBuffer.h"
#include "ConstantBuffer.h"
#include "set"

enum class RenderStage : UINT
{
	VS = 0,
	PS = VS + 1,
	GS = PS + 1,
	CS = GS + 1,
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
	
	D3D11_VIEWPORT viewport;

	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11GeometryShader> geometryShader;
	ComPtr<ID3D11DomainShader> domainShader;
	ComPtr<ID3D11PixelShader> pixelShader;

	ComPtr<ID3D11ComputeShader> computeShader;

	ComPtr<ID3D11InputLayout> inputlayout;

	shared_ptr<PrimitiveBuffer> primitiveBuffer;

	ComPtr<ID3D11RenderTargetView> rtv;
	ComPtr<ID3D11DepthStencilView> dsv;

	std::array<ComPtr<ID3DBlob>, StageCount> blobs;



	RenderPassState state;

	D3D11_SHADER_DESC shaderDesc;

	using BindDesc = D3D11_SHADER_INPUT_BIND_DESC;
	using BindDescList = std::vector<BindDesc>;
	std::unordered_map<std::string, BindDesc> bindDescMap;
	std::unordered_map<std::string, BindDesc> constantBindDescMap;

	std::unordered_map<std::string, ConstantBuffer> constantBuffers;
	std::unordered_map<std::string, std::vector<RenderStage>> stageListPerSemantic;

	std::set<std::string, ID3D11Resource*> userSRV;


	bool BindSRV(std::string name, ID3D11ShaderResourceView* resource);
	void BindRenderTargets(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv);
	void BindDepthStencilState(ID3D11DepthStencilState* state);
	bool BindSampler(std::string name, ID3D11SamplerState* resource);

	RenderPass(std::wstring shaderName , RenderPassState state);

	void Initialize(std::wstring shaderName, RenderPassState state);
	void InitializeInputLayout(D3D11_SHADER_DESC shaderDesc, ID3D11ShaderReflection* reflection, ID3DBlob* blob);

	void DrawIndexed(UINT indexCount, UINT StartIndexLocation, INT BseVertexLocation);

	void BeginDraw();
	void EndDraw();

private:
	void BindConstantBufferToStage(std::string name, const ConstantBuffer& cbuffer);

	ID3DBlob* GetBlob(RenderStage stage);
	ID3DBlob** GetBlobAddressOf(RenderStage stage);

	/*
	set sample
	set shader resource ( csv , srv, uav
	*/
	

};

