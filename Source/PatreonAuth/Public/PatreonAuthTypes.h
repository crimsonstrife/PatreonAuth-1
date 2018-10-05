// Copyright (c) 2018 fivefingergames.

#pragma once
 
#include "PatreonAuthTypes.generated.h"

USTRUCT(BlueprintType)
struct PATREONAUTH_API FPatreonAccessToken
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString access_token;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString refresh_token;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString expires_in;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString scope;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString token_type;

	FPatreonAccessToken() {}
};

USTRUCT(BlueprintType)
struct PATREONAUTH_API FPatreonError
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString id;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString status;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString title;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString detail;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") int32 code;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString code_name;

	FPatreonError() {}
};

USTRUCT(BlueprintType)
struct PATREONAUTH_API FPatreonErrors
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") TArray<FPatreonError> errors;

	FPatreonErrors() {}
};

USTRUCT(BlueprintType)
struct PATREONAUTH_API FPatreonIdentityDataAttrributesSocialConnections
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString deviantart;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString discord;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString facebook;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString spotify;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString twitch;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString twitter;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString youtube;

	FPatreonIdentityDataAttrributesSocialConnections() {}
};

USTRUCT(BlueprintType)
struct PATREONAUTH_API FPatreonIdentityDataAttrributes
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString email;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString is_email_verified;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString first_name;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString last_name;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString full_name;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString created;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString about;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") int32 like_count;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") bool can_see_nsfw;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") bool hide_pladges;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString image_url;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString thumb_url;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FPatreonIdentityDataAttrributesSocialConnections social_connections;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString url;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString vanity;

	FPatreonIdentityDataAttrributes() {}
};

USTRUCT(BlueprintType)
struct PATREONAUTH_API FPatreonIdentityData
{
    GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString id;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString type;
    UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FPatreonIdentityDataAttrributes attributes;

	FPatreonIdentityData() {}
};

USTRUCT(BlueprintType)
struct PATREONAUTH_API FPatreonIdentityIncluded
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString id;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString type;

	FPatreonIdentityIncluded() {}
};

USTRUCT(BlueprintType)
struct PATREONAUTH_API FPatreonIdentity
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FPatreonIdentityData data;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") TArray<FPatreonIdentityIncluded> included;

	FPatreonIdentity() {}
};

USTRUCT(BlueprintType)
struct PATREONAUTH_API FPatreonMemberDataAttrributes
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString full_name;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString patron_status;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") bool is_follower;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") int32 will_pay_amount_cents;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") int32 currently_entitled_amount_cents;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") int32 lifetime_support_cents;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString last_charge_status;

	FPatreonMemberDataAttrributes() {}
};

USTRUCT(BlueprintType)
struct PATREONAUTH_API FPatreonMemberData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString id;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString type;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FPatreonMemberDataAttrributes attributes;

	FPatreonMemberData() {}
};

USTRUCT(BlueprintType)
struct PATREONAUTH_API FPatreonMemberTierAttributes
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString title;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString description;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString url;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString discord_role_ids;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") bool requires_shipping;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") bool published;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString published_at;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString unpublished_at;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString created_at;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString edited_at;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString image_url;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") int32 patron_count;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") int32 remaining;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") int32 user_limit;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") int32 post_count;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") int32 amount_cents;

	FPatreonMemberTierAttributes() {}
};

USTRUCT(BlueprintType)
struct PATREONAUTH_API FPatreonMemberIncluded
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString id;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FString type;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FPatreonMemberTierAttributes attributes;

	FPatreonMemberIncluded() {}
};

USTRUCT(BlueprintType)
struct PATREONAUTH_API FPatreonMember
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") FPatreonMemberData data;
	UPROPERTY(BlueprintReadOnly, Category = "Patreon Auth") TArray<FPatreonMemberIncluded> included;

	FPatreonMember() {}
};