// Copyright 2024 Cajun Pro LLC. All Rights Reserved.

#pragma once

struct EDGEGAPSUBSYSTEM_API FJsonUtils
{
	FJsonUtils() = delete;
	
	static FString JsonObjectToString(const TSharedPtr<FJsonObject>& JsonObject)
	{
		FString Out;
		const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Out);
		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
		return Out;
	}

	static TSharedPtr<FJsonObject> StringToJsonObject(const FString& JsonString)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		// ReSharper disable once CppTooWideScopeInitStatement
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			return JsonObject;
		return TSharedPtr<FJsonObject>();
	}

	static TSharedPtr<FJsonObject> MapToJsonObject(const TMap<FString,FString>& Map)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		TArray<FString> Keys;
		Map.GenerateKeyArray(Keys);
		for (int32 i = 0; i < Keys.Num(); i++)
		{
			JsonObject->SetStringField(Keys[i], Map[Keys[i]]);
		}
		return JsonObject;
	}
};
