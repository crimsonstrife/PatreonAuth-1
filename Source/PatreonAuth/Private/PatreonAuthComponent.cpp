// Copyright (c) 2018 fivefingergames.

#include "PatreonAuthComponent.h"
#include "CoreMinimal.h"
#include "JsonUtilities.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

DEFINE_LOG_CATEGORY(LogPatreonAuth);

UPatreonAuthComponent::UPatreonAuthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

#pragma region Public

#pragma region Blueprint Interaction

void UPatreonAuthComponent::Authenticate(UWebBrowser* WebBrowser)
{
    if (WebBrowser)
    {
        m_WebBrowser = WebBrowser;
        FString url = UPatreonApi::GetAuthenticationUrl(ClientId);
        m_WebBrowser->OnUrlChanged.AddDynamic(this, &UPatreonAuthComponent::HandleOnUrlChanged);
        m_WebBrowser->LoadURL(url);
    }
    else
    {
        FPatreonError error;
        error.status = "404";
        error.title = "Browser Widget not found";
        error.detail = "Browser widget could not be found";

        FPatreonErrors errors;
        m_LastErrors = errors;
        m_LastErrors.errors.Add(error);

        LogError(m_LastErrors);
    }
}

void UPatreonAuthComponent::FetchCreatorCampaign() 
{
	//
}

void UPatreonAuthComponent::IsUserPatron()
{
	//
}

FPatreonErrors UPatreonAuthComponent::GetLastErrors() const
{
    return m_LastErrors;
}

UWebBrowser* UPatreonAuthComponent::GetWebBrowser() const
{
    return m_WebBrowser;
}

FPatreonIdentity UPatreonAuthComponent::GetUser() const
{
	return m_PatreonIdentity;
}

#pragma endregion // Blueprint Interaction

#pragma endregion // Public

#pragma region Protected

void UPatreonAuthComponent::LogError(const FPatreonErrors& PatreonErrors) const
{
	for (FPatreonError error : PatreonErrors.errors)
	{
		FString logMessage = error.title + "(" + error.status + "): " + error.title;
		UE_LOG(LogPatreonAuth, Error, TEXT("PatreonAuth: %s"), *logMessage);
	}
}

#pragma endregion // Protected

#pragma region Private

#pragma  region Web Browser Widget

void UPatreonAuthComponent::HandleOnUrlChanged(const FText& InText)
{
    FString url = InText.ToString();
    if(url.Contains(UPatreonApi::SINGLE_USE_TOKEN_URI_CONTAINS) == true)
    {
		OnSingleUseCodePageLoaded.Broadcast();
		FString singleUseCode = UPatreonApi::ExractSingleUseCode(url);
        ExecuteGetAccessTokenRequest(singleUseCode);
    }
    else
    {
        OnAuthenticationPageLoaded.Broadcast();
    }
}

#pragma endregion //Web Browser Widget

#pragma region Twitch HTTP API

void UPatreonAuthComponent::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    // When the response comes in we first check if it is valid.
    if(UPatreonApi::IsResponseValid(Response, bWasSuccessful) == true)
    {
        // Based on the last endpoint we send a request to we handle the response.
        switch(m_LastEndpoint)
        {
		case EPatreonEndpoint::Campaign:
			m_LastEndpoint = EPatreonEndpoint::None;
			//HandleGetCampaignResponse(Request, Response);

			break;

		case EPatreonEndpoint::Token:
			m_LastEndpoint = EPatreonEndpoint::None;
			HandleGetAccessTokenResponse(Request, Response);
			
			break;

		case EPatreonEndpoint::Identity:
			m_LastEndpoint = EPatreonEndpoint::None;
			//HandleGetIdentityResponse(Request, Response);
			
			break;

		case EPatreonEndpoint::Members:
			m_LastEndpoint = EPatreonEndpoint::None;
			//HandleGetMemberResponse(Request, Response);
			
			break;

		default:
			break;
        }
    }
    else
    {
        FString responseBody = Response->GetContentAsString();
        FJsonObjectConverter::JsonObjectStringToUStruct<FPatreonErrors>(responseBody, &m_LastErrors, 0, 0);

        switch(m_LastEndpoint)
        {
            case EPatreonEndpoint::Token:
                LogError(m_LastErrors);
                //OnUserFollowingChannel.Broadcast(false, m_TwitchFollow);
                break;

            case EPatreonEndpoint::Identity:
                LogError(m_LastErrors);
                //OnUserSubscribedToChannel.Broadcast(false, m_TwitchSubscription);
                break;

            case EPatreonEndpoint::Members:
                LogError(m_LastErrors);
                //OnUserSubscribedToChannel.Broadcast(false, m_TwitchSubscription);
                break;
        }
    }
}

#pragma endregion // HTTP API

#pragma region Twitch API Endpoints

void UPatreonAuthComponent::ExecuteGetAccessTokenRequest(const FString& SingleUseCode)
{
        // Create a request with the correct endpoint.
        TSharedRef<IHttpRequest> request = UPatreonApi::CreateHttpRequest("", UPatreonApi::TOKEN_ENDPOINT, EPatreonHttpVerb::Get, true);
        FString payload = UPatreonApi::CreateTokenRequestPayload(SingleUseCode, ClientId, ClientSecret);
        request->SetContentAsString(payload);
        
        request->OnProcessRequestComplete().BindUObject(this, &UPatreonAuthComponent::OnResponseReceived);
    
        // Set the last endpoint so that the response handler knows what to do.
        m_LastEndpoint = EPatreonEndpoint::Token;
    
        // Send the request.
        request->ProcessRequest();
}

void UPatreonAuthComponent::HandleGetAccessTokenResponse(FHttpRequestPtr Request, FHttpResponsePtr Response)
{
    // Get the response body as a string.
    const FString responseBody = Response->GetContentAsString();
    
    // And convert the body to a struct.
    if(FJsonObjectConverter::JsonObjectStringToUStruct<FPatreonAccessToken>(responseBody, &m_AccessToken, 0, 0) == true)
    {
        OnAuthenticated.Broadcast(true);
    }
    else
    {
        OnAuthenticated.Broadcast(false);
    }
}

//void UPatreonAuthComponent::HandleGetUserResponse(FHttpRequestPtr Request, FHttpResponsePtr Response)
//{
//    // Get the response body as a string.
//    const FString responseBody = Response->GetContentAsString();
//
//    // And convert the body to a struct.
//    if(FJsonObjectConverter::JsonObjectStringToUStruct<FTwitchUser>(responseBody, &m_TwitchUser, 0, 0) == true)
//    {
//        OnUserAuthenticated.Broadcast(true);
//    }
//    else
//    {
//        OnUserAuthenticated.Broadcast(false);
//    }
//}
//
//void UPatreonAuthComponent::ExecuteGetChannelRequest(const FString& ChannelName)
//{
//    const FString endpoint = UPatreonApi::CHANNEL_ENDPOINT + ChannelName;
//    TSharedRef<IHttpRequest> request = UPatreonApi::CreateHttpRequest(ClientId, m_AccessToken, endpoint, EHttpVerb::Get);
//    request->OnProcessRequestComplete().BindUObject(this, &UPatreonAuthComponent::OnResponseReceived);
//
//    m_LastEndpoint = EEndpoint::Channels;
//
//    request->ProcessRequest();
//}
//
//void UPatreonAuthComponent::HandleGetChannelResponse(FHttpRequestPtr Request, FHttpResponsePtr Response)
//{
//    // Get the response body as a string.
//    FString responseBody = Response->GetContentAsString();
//    responseBody = UPatreonApi::ExtractTwitchChannelUserFromResponseBody(responseBody);
//
//    // And convert the body to a struct.
//    if(FJsonObjectConverter::JsonObjectStringToUStruct<FTwitchChannelUser>(responseBody, &m_TwitchChannelUser, 0, 0) == true)
//    {
//        switch(m_TwitchRequest)
//        {
//            case ETwitchRequest::Subscription: ExecuteCheckUserSubscriptionRequest(m_TwitchUser, m_TwitchChannelUser);
//                break;
//            case ETwitchRequest::Following: ExecuteCheckUserFollowingRequest(m_TwitchUser, m_TwitchChannelUser);
//                break;
//        }
//
//        m_TwitchRequest = ETwitchRequest::None;
//    }
//    else
//    {
//        FTwitchError twitchError;
//        twitchError.error = "Not Found";
//        twitchError.status = 404;
//        twitchError.message = "Channel could not be found";
//
//        m_LastError = twitchError;
//
//        LogError(m_LastError);
//        OnUserSubscribedToChannel.Broadcast(false, m_TwitchSubscription);
//    }
//}
//
//void UPatreonAuthComponent::ExecuteCheckUserSubscriptionRequest(const FTwitchUser& TwitchUser, const FTwitchChannelUser& TwitchChannel)
//{
//    FString endpoint = UPatreonApi::SUBSCRIPTION_ENDPOINT;
//    endpoint = endpoint.Replace(TEXT("$1"), *TwitchUser._id);
//    endpoint = endpoint.Replace(TEXT("$2"), *TwitchChannel._id);
//
//    TSharedRef<IHttpRequest> request = UPatreonApi::CreateHttpRequest(ClientId, m_AccessToken, endpoint, EHttpVerb::Get);
//    request->OnProcessRequestComplete().BindUObject(this, &UPatreonAuthComponent::OnResponseReceived);
//
//    m_LastEndpoint = EEndpoint::Subscriptions;
//
//    request->ProcessRequest();
//}
//
//void UPatreonAuthComponent::HandleCheckUserSubscriptionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response)
//{
//    const FString responseBody = Response->GetContentAsString();
//
//    // And convert the body to a struct.
//    if(FJsonObjectConverter::JsonObjectStringToUStruct<FTwitchSubscription>(responseBody, &m_TwitchSubscription, 0, 0) == true)
//    {
//        OnUserSubscribedToChannel.Broadcast(true, m_TwitchSubscription);
//    }
//}
//
//void UPatreonAuthComponent::ExecuteCheckUserFollowingRequest(const FTwitchUser& TwitchUser, const FTwitchChannelUser& TwitchChannel)
//{
//    FString endpoint = UPatreonApi::FOLLOWING_ENDPOINT;
//    endpoint = endpoint.Replace(TEXT("$1"), *TwitchUser._id);
//    endpoint = endpoint.Replace(TEXT("$2"), *TwitchChannel._id);
//
//    TSharedRef<IHttpRequest> request = UPatreonApi::CreateHttpRequest(ClientId, m_AccessToken, endpoint, EHttpVerb::Get);
//    request->OnProcessRequestComplete().BindUObject(this, &UPatreonAuthComponent::OnResponseReceived);
//
//    m_LastEndpoint = EEndpoint::Following;
//
//    request->ProcessRequest();
//}
//
//void UPatreonAuthComponent::HandleCheckUserFollowingResponse(FHttpRequestPtr Request, FHttpResponsePtr Response)
//{
//    const FString responseBody = Response->GetContentAsString();
//
//    // And convert the body to a struct.
//    if(FJsonObjectConverter::JsonObjectStringToUStruct<FTwitchFollow>(responseBody, &m_TwitchFollow, 0, 0) == true)
//    {
//        OnUserFollowingChannel.Broadcast(true, m_TwitchFollow);
//    }
//}

#pragma endregion // Twitch API Endpoints

#pragma endregion // Private