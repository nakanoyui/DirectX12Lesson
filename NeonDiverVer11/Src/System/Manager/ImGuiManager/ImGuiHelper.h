#pragma once

namespace ImGuiHelper
{
	inline const Math::Vector2 InputFloat2(const char* text, Math::Vector2& Out)
	{
		float temp[] = { Out.x, Out.y };

		ImGui::InputFloat2(text, temp);
		ImGui::Separator();

		Out = Math::Vector2(temp[0], temp[1]);
		return Math::Vector2(temp[0], temp[1]);
	}

	inline const Math::Vector3 InputFloat3(const char* text, Math::Vector3& Out)
	{
		float temp[] = { Out.x, Out.y, Out.z };

		ImGui::InputFloat3(text, temp);
		ImGui::Separator();

		Out = Math::Vector3(temp[0], temp[1], temp[2]);
		return Math::Vector3(temp[0], temp[1], temp[2]);
	}

	inline const Math::Vector3 SliderFloat3(const char* text, Math::Vector3& Out, float min, float max)
	{
		float temp[] = { Out.x, Out.y, Out.z };

		ImGui::SliderFloat3(text, temp, min, max);
		ImGui::Separator();

		Out = Math::Vector3(temp[0], temp[1], temp[2]);
		return Math::Vector3(temp[0], temp[1], temp[2]);
	}

	inline const Math::Vector3 ColorEdit3(const char* text, Math::Vector3& Out)
	{
		float temp[] = { Out.x, Out.y, Out.z };

		ImGui::ColorEdit3(text, temp);
		ImGui::Separator();

		Out = { temp[0], temp[1], temp[2] };
		return Math::Vector3(temp[0], temp[1], temp[2]);
	}

	template<typename T>
	inline T CheckData(nlohmann::json& json, T& Out, const std::string& searchName, T falseData)
	{
		nlohmann::json::iterator checkIdIt = json.find(searchName);

		if (checkIdIt != json.end())
		{
			Out = json.at(searchName);
		}
		else
		{
			Out = falseData;
		}

		return Out;
	}

	inline Math::Vector2 CheckDataVector2(nlohmann::json& json, Math::Vector2& Out, const std::string& searchName, Math::Vector2 falseData)
	{
		nlohmann::json::iterator checkIdIt = json.find(searchName);

		if (checkIdIt != json.end())
		{
			Out.x = json.at(searchName)[0];
			Out.y = json.at(searchName)[1];
		}
		else
		{
			Out = falseData;
		}

		return Out;
	}

	inline Math::Vector3 CheckDataVector3(nlohmann::json& json, Math::Vector3& Out, const std::string& searchName, Math::Vector3 falseData)
	{
		nlohmann::json::iterator checkIdIt = json.find(searchName);

		if (checkIdIt != json.end())
		{
			Out.x = json.at(searchName)[0];
			Out.y = json.at(searchName)[1];
			Out.z = json.at(searchName)[2];
		}
		else
		{
			Out = falseData;
		}

		return Out;
	}

	constexpr int InputTextSize = 64;
}