// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkingThread.h"

NetworkingThread::NetworkingThread()
{
	
}

NetworkingThread::~NetworkingThread()
{
	
}

FSocketThread::FSocketThread(const FString& InIPAddress, const int32 InPortNumber)
    : IPAddress(InIPAddress)
    , PortNumber(InPortNumber)
    , Socket(nullptr)
    , IsConnected(false)
{}

bool FSocketThread::Init()
{
    // Create a socket object
    Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

    // Connect to the server
    TSharedPtr<FInternetAddr> ServerAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
    bool bIsValid;
    ServerAddress->SetIp(*IPAddress, bIsValid);
    ServerAddress->SetPort(PortNumber);
    IsConnected = Socket->Connect(*ServerAddress);
    
    return IsConnected;
}

uint32 FSocketThread::Run()
{
    while (IsConnected)
    {
        // Receive data from the server
        uint8 Buffer[1024];
        int32 BytesReceived = 0;
        IsConnected = Socket->Recv(Buffer, sizeof(Buffer), BytesReceived);
        if (IsConnected && BytesReceived > 0) {
            FString ReceivedData = FString(UTF8_TO_TCHAR(Buffer));
            UE_LOG(LogTemp, Display, TEXT("Received data: %s"), *ReceivedData);
        }
		IsConnected = Socket->Send(Buffer, sizeof(Buffer), BytesReceived);
        


        
    }

    return 0;
}

void FSocketThread::Stop()
{
    // Close the socket connection
    if (Socket)
    {
        Socket->Close();
        delete Socket;
        Socket = nullptr;
    }
}