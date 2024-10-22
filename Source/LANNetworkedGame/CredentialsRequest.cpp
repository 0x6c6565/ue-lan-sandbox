// Fill out your copyright notice in the Description page of Project Settings.

#include "CredentialsRequest.h"

#include "Misc/Paths.h"

UCredentialsRequest* UCredentialsRequest::OnCredentialsRequest(const FString& Exe, const FString& Credentials)
{
	UCredentialsRequest* Node = NewObject<UCredentialsRequest>();
	
	//Node->CredentialsPath = !Credentials.IsEmpty() ? FPaths::ConvertRelativePathToFull(Credentials) : Credentials;
	//if (!Exe.IsEmpty() && FPaths::FileExists(Exe))
	//{
	//	Node->ExePath = Exe;
	//}
	//else
	//{
	//	Node->ExePath = FString();
	//	Node->CredentialsPath.Empty();

	//	UE_LOG(LogTemp, Error, TEXT("COULD NOT FIND %s"), *Exe);
	//}

	Node->ExePath = Exe;
	Node->CredentialsPath = Credentials;

	Node->ReturnCode = 0;

	return Node;
}

void UCredentialsRequest::Activate()
{
	//if (!ExePath.IsEmpty())
	if (ValidatePaths())
	{
		AsyncTask(ENamedThreads::AnyThread, [this]()
			{
				//ProcessHandle = FPlatformProcess::CreateProc(*ExePath, *CredentialsPath, false, true, false, nullptr, 0, nullptr, nullptr, nullptr);
				ProcessHandle = FPlatformProcess::CreateProc(*ExePath, *CredentialsPath, false, false, false, nullptr, 0, nullptr, nullptr, nullptr);
				if (ProcessHandle.IsValid())
				{
					FPlatformProcess::WaitForProc(ProcessHandle);

					if (FPlatformProcess::GetProcReturnCode(ProcessHandle, &ReturnCode))
					{
						if (0 == ReturnCode)
						{
							Success.Broadcast();
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("Credentials Request return an error code (%d)"), ReturnCode);

							Failure.Broadcast();
						}
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Credentials Request failed to retrieve return code."));

						Failure.Broadcast();
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Credentials Request process handle was invalid."));

					Failure.Broadcast();
				}
			});
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("EXECUTABLE IS EMPTY!"));

		Failure.Broadcast();
	}
}

bool UCredentialsRequest::ValidatePaths()
{
	if (!ExePath.IsEmpty())
	{
		if (FPaths::FileExists(ExePath))
		{
			if (!CredentialsPath.IsEmpty())
			{
				CredentialsPath = FPaths::ConvertRelativePathToFull(CredentialsPath);
			}

			return true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Credentials Request executable path does not exist!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Credentials Request executable path is empty!"));
	}

	return false;
}