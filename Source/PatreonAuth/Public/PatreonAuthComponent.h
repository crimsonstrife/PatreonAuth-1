// Copyright (c) 2018 fivefingergames.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "WebBrowser.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "PatreonAuthTypes.h"
#include "PatreonApi.h"
#include "PatreonAuthComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPatreonAuth, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAuthenticationPageLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSingleUseCodePageLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreatorCampaignFetched);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthenticated, bool, bAuthenticated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnIsUserPatron, bool, bPatron, FPatreonMember, PatreonMember);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
/**
 * @class   TWITCHAUTH_API TwitchAuthComponent.h
 *          D:\Projects\fivefingergames\FFGPlugins\Plugins\TwitchAuth\Source\TwitchAuth\Public\TwitchAuthComponent.h
 *
 * @brief   Twitch authentication actor component.
 */
class PATREONAUTH_API UPatreonAuthComponent : public UActorComponent
{
    GENERATED_BODY()

public:

    /**
     * @fn  UTwitchAuthComponent::UTwitchAuthComponent();
     *
     * @brief   Default constructor.
     */
    UPatreonAuthComponent();

    /** Client ID from your Patreon client. */
    UPROPERTY(EditDefaultsOnly, Category = "Twitch Auth")
    FString ClientId;

    /** Client secret from your Patreon client. */
	UPROPERTY(EditDefaultsOnly, Category = "Twitch Auth")
	FString ClientSecret;

	/** Client creator access token from your Patreon client. */
	UPROPERTY(EditDefaultsOnly, Category = "Twitch Auth")
	FString CreatorAccessToken;

	/** Client creator refresh token from your Patreon client. */
	UPROPERTY(EditDefaultsOnly, Category = "Twitch Auth")
	FString CreatorRefreshToken;

	/** Will be fired when the creator campaign fetching process finishes. */
	UPROPERTY(BlueprintAssignable, Category = "Twitch Auth|Events")
	FOnCreatorCampaignFetched OnCreatorCampaignFetched;

    /** Will be fired when the authentication process finishes. */
    UPROPERTY(BlueprintAssignable, Category = "Twitch Auth|Events")
    FOnAuthenticated OnAuthenticated;

    /** Will be fired when the authentication page has finished loading in the given browser. */
    UPROPERTY(BlueprintAssignable, Category = "Twitch Auth|Events")
    FOnAuthenticationPageLoaded OnAuthenticationPageLoaded;

    /** Will be fired when the single use code page has finished loading in the given browser. */
    UPROPERTY(BlueprintAssignable, Category = "Twitch Auth|Events")
	FOnSingleUseCodePageLoaded OnSingleUseCodePageLoaded;

    /**
     * @fn	void UPatreonAuthComponent::Authenticate(UWebBrowser* WebBrowser);
     *
     * @brief	Starts the authentication process with a given browser widget.
     *
     * @param [in,out]	WebBrowser	Web browser that will be used to display the Twitch
     * 								authentication web page.
     */
	UFUNCTION(BlueprintCallable, Category = "Twitch Auth", meta = (ToolTip = "Start the authentication process with a given browser widget."))
    void Authenticate(UWebBrowser* WebBrowser);

	/**
	 * @fn	void UPatreonAuthComponent::FetchCreatorCampaign();
	 *
	 * @brief	Fetches creators campaign.
	 */
	UFUNCTION(BlueprintCallable, Category = "Twitch Auth", meta = (ToolTip = "Fetches the campaign of the creator that added a Patreon client. This campaign is then store on the component to check against the authenticated user."))
	void FetchCreatorCampaign();

    /**
     * @fn	FPatreonIdentity UPatreonAuthComponent::GetUser() const;
     *
     * @brief	Returns the authenticated Patreon user.
     *
     * @return	The authenticated Twitch user.
     */
    UFUNCTION(BlueprintPure, Category = "Twitch Auth", meta = (ToolTip = "Returns the authenticated Twitch user."))
    FPatreonIdentity GetUser() const;

    /**
     * @fn	void UPatreonAuthComponent::IsUserPatron() const;
     *
     * @brief	Checks if the authenticated user is a patron of your campaign.
     */
    UFUNCTION(BlueprintCallable, Category = "Twitch Auth", meta = (ToolTip = "Check if the authenticated Twitch user is subscribed to a given channel."))
	void IsUserPatron();

	/**
	 * @fn	FPatreonErrors UPatreonAuthComponent::GetLastErrors() const;
	 *
	 * @brief	Returns the last errors.
	 *
	 * @return	The last error.
	 */
    UFUNCTION(BlueprintPure, Category = "Twitch Auth", meta = (ToolTip = "Returns the last error."))
	FPatreonErrors GetLastErrors() const;

    /**
     * @fn  UWebBrowser* UTwitchAuthComponent::GetWebBrowser() const;
     *
     * @brief   Returns the used web browser.
     *
     * @return  Null if it fails, else the web browser.
     */
    UFUNCTION(BlueprintPure, Category = "Twitch Auth", meta = (ToolTip = "Returns the used web browser."))
    UWebBrowser* GetWebBrowser() const;

protected:

    #pragma region Blueprint Interaction

    /** The last error */
    FPatreonErrors m_LastErrors;

    /** The access token */
    FPatreonAccessToken m_AccessToken;

    #pragma endregion // Blueprint Interaction

    /**
     * @fn  void UTwitchAuthComponent::LogError(const FTwitchError& twitchError) const;
     *
     * @brief   Logs an given error.
     *
     * @param   twitchError The twitch error.
     */
    void LogError(const FPatreonErrors& PatreonErrors) const;

    #pragma region HTTP API

    /**
     * @fn  void UTwitchAuthComponent::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
     *
     * @brief   Will be called when every a request sends back an response. This is the place to handle any incoming responses.
     *
     * @param   Request         The request.
     * @param   Response        The response.
     * @param   bWasSuccessful  True if the was operation was a success, false if it failed.
     */
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

    #pragma endregion // HTTP API;

private:

    #pragma region Web Browser Widget

    /** @brief   The web browser that is used to display the authentication pages. */
    UWebBrowser* m_WebBrowser;

    /**
     * @fn  void UTwitchAuthComponent::HandleOnUrlChanged(const FText& InText);
     *
     * @brief   Handles the on URL change from the web browser.
     *
     * @param   InText  The new URL.
     */
    UFUNCTION()
    void HandleOnUrlChanged(const FText& InText);

    #pragma endregion // Web Browser Widget

    #pragma region Patreon API Endpoints

	/** The patreon request currently being executed, this is not mapped to any endpoints. This is the wording within this client. */
	EPatreonRequest m_PatreonRequest = EPatreonRequest::None;

    /** The last endpoint a request was send to. */
    EEndpoint m_LastEndpoint = EEndpoint::None;

    /** The authenticated Twitch user from the Authentication process. */
    FPatreonIdentity m_PatreonIdentity;

    /** The Twitch channel from the IsUserSubscribedToChannel process. */
    FPatreonMember m_PatreonMember;
	
	void ExecuteGetCampaignRequest();
	void HandleGetCampaignResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);

    void ExecuteGetAccessTokenRequest(const FString& SingleUseCode);
    void HandleGetAccessTokenResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);

    void ExecuteGetIdentityRequest();
    void HandleGetIdentityResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);

    void ExecuteGetMemberRequest(const FString& MembershipId);
    void HandleGetMemberResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);


    #pragma endregion // Twitch API Endpoints

};
