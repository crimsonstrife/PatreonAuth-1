// Copyright (c) 2018 fivefingergames.

#include "PatreonApi.h"

const FString UPatreonApi::API_BASE_URL = TEXT("https://www.patreon.com/api/oauth2");
const FString UPatreonApi::API_BASE_URL_V2 = TEXT("https://www.patreon.com/api/oauth2/v2");
const FString UPatreonApi::SINGLE_USE_CODE_KEY = TEXT("code=");
const FString UPatreonApi::SINGLE_USE_TOKEN_URI_CONTAINS = TEXT("https://localhost/?code");

const FString UPatreonApi::TOKEN_ENDPOINT = TEXT("");
const FString UPatreonApi::IDENTITY_ENDPOINT = TEXT("/identity");
const FString UPatreonApi::MEMBERS_ENDPOINT = TEXT("/members/{0}?fields%5Bmember%5D=full_name,patron_status,is_follower,lifetime_support_cents,currently_entitled_amount_cents,will_pay_amount_cents&fields%5Btier%5D=amount_cents,user_limit,remaining,description,requires_shipping,created_at,url,patron_count,post_count,discord_role_ids,title,image_url,edited_at,published,published_at,unpublished_at&include=currently_entitled_tiers");


TSharedRef<IHttpRequest> UPatreonApi::CreateHttpRequest(const FString& AccessToken, const FString& Endpoint, EHttpVerb Verb, const bool bFormDataUrlEncoded)
{
    FHttpModule* http = &FHttpModule::Get();

    // First let's created the result.
    TSharedRef<IHttpRequest> result = http->CreateRequest();

    // Set the endpoint URL and the authorization header.
    FString url;
    if (Endpoint == TOKEN_ENDPOINT)
    {
        url = UPatreonApi::API_BASE_URL + Endpoint;
    }
    else
    {
        url = UPatreonApi::API_BASE_URL_V2 + Endpoint;
        result->SetHeader(TEXT("Authorization"), TEXT("Bearer " + AccessToken));
    }    

    result->SetURL(url);
	if (bFormDataUrlEncoded == true) 
    {
		result->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	}

    // Last but not least set the HTTP verb.
    FString verbStr = UPatreonApi::GetHttpVerbStr(Verb);
    result->SetVerb(verbStr);

    // And finally return the thing.
    return result;
}

FString UPatreonApi::CreateTokenRequestPayload(const FString& SingleUseCode, const FString& ClientId, const FString& ClientSecret)
{

    FString result;
    result += "code=" + SingleUseCode;
    result += "&grant_type=authorization_code";
    result += "&client_id=" + ClientId;
    result += "&client_secret=" + ClientSecret;
    result += "&redirect_uri=https://localhost";

    return result;
}

FString UPatreonApi::ExractSingleUseCode(const FString& RedirectUri)
{
    FString result = "";

    // First find the hash that marks the access token and 
    // add the key string length to the found index.
    int32 leftIndex = RedirectUri.Find("?");
    leftIndex += SINGLE_USE_CODE_KEY.Len() + 1;

    int32 rightIndex = RedirectUri.Find("&");

    int32 charCount = rightIndex - leftIndex;
    result = RedirectUri.Mid(leftIndex, charCount);

    return result;
}

FString UPatreonApi::GetAuthenticationUrl(const FString& ClientId)
{
    FString Url = "https://www.patreon.com/oauth2/authorize";
	Url += "?response_type=code";
	Url += "&client_id=" + ClientId;
    Url += "&redirect_uri=https://localhost";
    Url += "&scope=campaigns identity identity.memberships";

    return Url;
}

FString UPatreonApi::GetIdentityEndpointWithInclude(EEndpointInclude EndpointInclude)
{
	FString result = IDENTITY_ENDPOINT;

	switch (EndpointInclude)
	{
	case EEndpointInclude::Memberships: result += "?include=memberships";
		break;
	case EEndpointInclude::Campaign: result += "?include=campaign";
		break;
	}

	return result;
}

FString UPatreonApi::GetHttpVerbStr(EHttpVerb Verb)
{
    // Init the result.
    FString result = "";

    // Set the result variable based on the given enum value.
    switch(Verb)
    {
        case EHttpVerb::Get: result = "GET";
            break;
        case EHttpVerb::Post: result = "POST";
            break;
        case EHttpVerb::Put: result = "PUT";
            break;
        case EHttpVerb::Patch: result = "PATCH";
            break;
        case EHttpVerb::Delete: result = "DELETE";
            break;
        default:
            break;
    }

    // Finally return the result.
    return result;
}

bool UPatreonApi::IsResponseValid(FHttpResponsePtr Response, bool bWasSuccessful)
{
    // First check the easy stuff. 
    if(bWasSuccessful == false || Response.IsValid() == false) return false;

    // If the response code is OK 200 return true.
    if(EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        return true;
    }
    else
    {
        // Otherwise we log a warning to the console and return false.
        UE_LOG(LogTemp, Warning, TEXT("Http Response returned error code: %d"), Response->GetResponseCode());
        return false;
    }
}