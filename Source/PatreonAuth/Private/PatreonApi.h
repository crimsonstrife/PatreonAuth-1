// Copyright (c) 2018 fivefingergames.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "PatreonApi.generated.h"

/**
* @enum    EHttpVerb
*
* @brief   Values that represent HTTP verbs.
*/
UENUM()
enum class EPatreonHttpVerb : uint8
{
    Get,
    Post,
    Put,
    Patch,
    Delete
};

/**
* @enum    EEndpoint
*
* @brief   Values that represent Patreon API endpoints.
*/
UENUM()
enum class EPatreonEndpoint : uint8
{
    None,
	Campaign,
	Authorize,
	Token,
    Identity,
	Members
};

/**
* @enum    EPatreonRequest
*
* @brief   Values that represent Patreon API request flow for this plugin.
*/
UENUM()
enum class EPatreonRequest : uint8
{
	None,
	CreatorCampaign,
	Token,
	Identity,
	Members
};

UENUM()
enum class EEndpointInclude : uint8
{
	Memberships,
	Campaign
};

/**
 * 
 */
UCLASS()
class PATREONAUTH_API UPatreonApi : public UObject
{
	GENERATED_BODY()
	
public:

    /** @brief	Base URL of the Patreon HTTP API. */
    static const FString API_BASE_URL;

    /** @brief	Base URL of the Patreon HTTP API v2. */
    static const FString API_BASE_URL_V2;

	/** @brief	The single use code key from the redirect URI. */
	static const FString SINGLE_USE_CODE_KEY;

    /** @brief   Substring contained in the redirect URI. */
    static const FString SINGLE_USE_TOKEN_URI_CONTAINS;

    /** @brief   The Patreon token API endpoint. */
    static const FString TOKEN_ENDPOINT;

    /** @brief   The Patreon identity API endpoint. */
    static const FString IDENTITY_ENDPOINT;

    /** @brief   The Patreon members API endpoint */
    static const FString MEMBERS_ENDPOINT;

    /**
     * @fn	static FString UPatreonApi::GetAuthenticationUrl(const FString& ClientId, bool ForceVerify);
     *
     * @brief	Gets authentication URL for the Patreon API.
     *
     * @param	ClientId   	Identifier for the client.
     *
     * @return	The authentication URL.
     */
    static FString GetAuthenticationUrl(const FString& ClientId);

    /**
     * @fn	static FString UPatreonApi::ExractSingleUseCode(const FString& RedirectUri);
     *
     * @brief	Extracts the access token from a given redirect URI.
     *
     * @param	RedirectUri	URI.
     *
     * @return	The extracted access token.
     */
    static FString ExractSingleUseCode(const FString& RedirectUri);

    /**
     * @fn	static FString UPatreonApi::GetHttpVerbStr(EHttpVerb Verb);
     *
     * @brief	Gets HTTP verb string.
     *
     * @param	Verb	The verb.
     *
     * @return	The HTTP verb string.
     */
    static FString GetHttpVerbStr(EPatreonHttpVerb Verb);

    /**
     * @fn	static bool UPatreonApi::IsResponseValid(FHttpResponsePtr Response, bool bWasSuccessful);
     *
     * @brief	Checks if a response is valid
     *
     * @param	Response	  	The response.
     * @param	bWasSuccessful	True if the operation was a success, false if it failed.
     *
     * @return	True if the response is valid, false if not.
     */
    static bool IsResponseValid(FHttpResponsePtr Response, bool bWasSuccessful);

	static FString GetIdentityEndpointWithInclude(EEndpointInclude EndpointInclude);

    /**
     * @fn	static TSharedRef<IHttpRequest> UPatreonApi::CreateHttpRequest(const FString& ClientId, const FString& AccessToken, const FString& Endpoint, EHttpVerb Verb, const bool bFormDataUrlEncoded);
     *
     * @brief	Creates HTTP request.
     *
     * @param	ClientId		   	Identifier for the client.
     * @param	AccessToken		   	The access token.
     * @param	Endpoint		   	The HTTP endpoint.
     * @param	Verb			   	The HTTP verb.
     * @param	bFormDataUrlEncoded	True if Content-Type "application/x-www-form-urlencoded" should be set in the header.
     *
     * @return	The new HTTP request.
     */
    static TSharedRef<IHttpRequest> CreateHttpRequest(const FString& AccessToken, const FString& Endpoint, EPatreonHttpVerb Verb, const bool bFormDataUrlEncoded);

    /**
     * @fn  static FString UPatreonApi::CreateTokenRequestPayload(const FString& SingleUseCode, const FString& ClientId, const FString& ClientSecret);
     *
     * @brief   Creates token request payload.
     *
     * @param   SingleUseCode   The single use code.
     * @param   ClientId        Identifier for the client.
     * @param   ClientSecret    The client secret.
     *
     * @return  The token request payload.
     */
    static FString CreateTokenRequestPayload(const FString& SingleUseCode, const FString& ClientId, const FString& ClientSecret);
	
};
