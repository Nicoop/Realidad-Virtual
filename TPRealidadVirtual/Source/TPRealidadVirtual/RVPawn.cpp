#include "RVPawn.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"
#include "Engine/DirectionalLight.h"
#include "Engine/PointLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/PointLightComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

ARVPawn::ARVPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	RaizEscena = CreateDefaultSubobject<USceneComponent>(TEXT("RaizEscena"));
	SetRootComponent(RaizEscena);

	RaizGema = CreateDefaultSubobject<USceneComponent>(TEXT("RaizGema"));
	RaizGema->SetupAttachment(RaizEscena);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaCono(TEXT("/Engine/BasicShapes/Cone.Cone"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaPlano(TEXT("/Engine/BasicShapes/Plane.Plane"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialPatron(TEXT("/Game/M_Patron.M_Patron"));

	ConoSuperior = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConoSuperior"));
	ConoSuperior->SetupAttachment(RaizGema);
	if (MallaCono.Succeeded())
	{
		ConoSuperior->SetStaticMesh(MallaCono.Object);
	}
	ConoSuperior->SetRelativeLocation(FVector(0.0f, 0.0f, 55.0f));
	ConoSuperior->SetRelativeScale3D(FVector(1.1f, 1.1f, 1.6f));

	ConoInferior = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConoInferior"));
	ConoInferior->SetupAttachment(RaizGema);
	if (MallaCono.Succeeded())
	{
		ConoInferior->SetStaticMesh(MallaCono.Object);
	}
	ConoInferior->SetRelativeLocation(FVector(0.0f, 0.0f, -55.0f));
	ConoInferior->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
	ConoInferior->SetRelativeScale3D(FVector(1.1f, 1.1f, 1.6f));

	PlanosPivote = CreateDefaultSubobject<USceneComponent>(TEXT("PlanosPivote"));
	PlanosPivote->SetupAttachment(RaizEscena);

	PlanoA = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlanoA"));
	PlanoA->SetupAttachment(PlanosPivote);
	if (MallaPlano.Succeeded())
	{
		PlanoA->SetStaticMesh(MallaPlano.Object);
	}
	PlanoA->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	PlanoA->SetRelativeScale3D(FVector(7.0f, 2.2f, 1.0f));
	PlanoA->SetCastShadow(false);

	PlanoB = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlanoB"));
	PlanoB->SetupAttachment(PlanosPivote);
	if (MallaPlano.Succeeded())
	{
		PlanoB->SetStaticMesh(MallaPlano.Object);
	}
	PlanoB->SetRelativeRotation(FRotator(90.0f, 65.0f, 0.0f));
	PlanoB->SetRelativeScale3D(FVector(7.0f, 2.2f, 1.0f));
	PlanoB->SetCastShadow(false);

	Camara = CreateDefaultSubobject<UCameraComponent>(TEXT("Camara"));
	Camara->SetupAttachment(RaizEscena);
	Camara->SetRelativeLocation(FVector(-950.0f, 0.0f, 320.0f));
	Camara->SetRelativeRotation(FRotator(-18.0f, 0.0f, 0.0f));

	MaterialBasePatron = MaterialPatron.Succeeded() ? MaterialPatron.Object : nullptr;
	MaterialDinamicoA = nullptr;
	MaterialDinamicoB = nullptr;

	EjeRotacion = FVector::UpVector;
	VelocidadBase = 45.0f;
	VelocidadActual = VelocidadBase;
	ContadorPatron = 0;
}

void ARVPawn::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInterface* Base = MaterialBasePatron ? MaterialBasePatron : UMaterial::GetDefaultMaterial(MD_Surface);

	MaterialDinamicoA = UMaterialInstanceDynamic::Create(Base, this);
	MaterialDinamicoB = UMaterialInstanceDynamic::Create(Base, this);

	if (PlanoA)
	{
		PlanoA->SetMaterial(0, MaterialDinamicoA);
	}
	if (PlanoB)
	{
		PlanoB->SetMaterial(0, MaterialDinamicoB);
	}

	AplicarPatron();

	ADirectionalLight* LuzClave = GetWorld()->SpawnActor<ADirectionalLight>(FVector::ZeroVector, FRotator(-45.0f, -35.0f, 0.0f));
	if (LuzClave)
	{
		LuzClave->GetLightComponent()->Intensity = 8.0f;
	}

	APointLight* LuzRelleno = GetWorld()->SpawnActor<APointLight>(FVector(-400.0f, 400.0f, 250.0f), FRotator::ZeroRotator);
	if (LuzRelleno)
	{
		UPointLightComponent* Componente = Cast<UPointLightComponent>(LuzRelleno->GetLightComponent());
		if (Componente)
		{
			Componente->Intensity = 18000.0f;
			Componente->AttenuationRadius = 1500.0f;
			Componente->SetLightColor(FLinearColor(0.35f, 0.65f, 1.0f));
		}
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 999999.0f, FColor::White, TEXT("Flechas: eje/sentido de rotacion | D: detener | R: reiniciar (cambia el patron)"));
	}
}

void ARVPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (VelocidadActual != 0.0f && PlanosPivote)
	{
		PlanosPivote->AddLocalRotation(FQuat(EjeRotacion.GetSafeNormal(), FMath::DegreesToRadians(VelocidadActual * DeltaSeconds)));
	}

	if (RaizGema)
	{
		RaizGema->AddLocalRotation(FQuat(FVector::UpVector, FMath::DegreesToRadians(9.0f * DeltaSeconds)));
	}
}

void ARVPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("FlechaArriba", IE_Pressed, this, &ARVPawn::GirarArriba);
	PlayerInputComponent->BindAction("FlechaAbajo", IE_Pressed, this, &ARVPawn::GirarAbajo);
	PlayerInputComponent->BindAction("FlechaIzquierda", IE_Pressed, this, &ARVPawn::GirarIzquierda);
	PlayerInputComponent->BindAction("FlechaDerecha", IE_Pressed, this, &ARVPawn::GirarDerecha);
	PlayerInputComponent->BindAction("Detener", IE_Pressed, this, &ARVPawn::Detener);
	PlayerInputComponent->BindAction("Reiniciar", IE_Pressed, this, &ARVPawn::Reiniciar);
}

void ARVPawn::GirarArriba()
{
	EjeRotacion = FVector::RightVector;
	VelocidadActual = VelocidadBase;
}

void ARVPawn::GirarAbajo()
{
	EjeRotacion = FVector::RightVector;
	VelocidadActual = -VelocidadBase;
}

void ARVPawn::GirarIzquierda()
{
	EjeRotacion = FVector::UpVector;
	VelocidadActual = VelocidadBase;
}

void ARVPawn::GirarDerecha()
{
	EjeRotacion = FVector::UpVector;
	VelocidadActual = -VelocidadBase;
}

void ARVPawn::Detener()
{
	VelocidadActual = 0.0f;
}

void ARVPawn::Reiniciar()
{
	if (PlanosPivote)
	{
		PlanosPivote->SetRelativeRotation(FRotator::ZeroRotator);
	}
	EjeRotacion = FVector::UpVector;
	VelocidadActual = VelocidadBase;
	ContadorPatron++;
	AplicarPatron();
}

void ARVPawn::AplicarPatron()
{
	TArray<TPair<FColor, FColor>> Paletas;
	Paletas.Add(TPair<FColor, FColor>(FColor(255, 115, 0), FColor(38, 217, 51)));
	Paletas.Add(TPair<FColor, FColor>(FColor(217, 26, 38), FColor(242, 217, 51)));
	Paletas.Add(TPair<FColor, FColor>(FColor(26, 140, 242), FColor(230, 230, 242)));

	const int32 Tipo = ContadorPatron % 3;
	const TPair<FColor, FColor>& Paleta = Paletas[(ContadorPatron / 3) % Paletas.Num()];

	UTexture2D* TexturaA = nullptr;
	UTexture2D* TexturaB = nullptr;

	if (Tipo == 0)
	{
		TexturaA = CrearTexturaAjedrez(Paleta.Key, Paleta.Value, 8, 128);
		TexturaB = CrearTexturaAjedrez(Paleta.Value, Paleta.Key, 8, 128);
	}
	else if (Tipo == 1)
	{
		TexturaA = CrearTexturaFranjas(Paleta.Key, Paleta.Value, 10, 128);
		TexturaB = CrearTexturaFranjas(Paleta.Value, Paleta.Key, 10, 128);
	}
	else
	{
		TexturaA = CrearTexturaAnillos(Paleta.Key, Paleta.Value, 6, 128);
		TexturaB = CrearTexturaAnillos(Paleta.Value, Paleta.Key, 6, 128);
	}

	if (MaterialDinamicoA && TexturaA)
	{
		MaterialDinamicoA->SetTextureParameterValue(FName("Patron"), TexturaA);
	}
	if (MaterialDinamicoB && TexturaB)
	{
		MaterialDinamicoB->SetTextureParameterValue(FName("Patron"), TexturaB);
	}
}

UTexture2D* ARVPawn::CrearTexturaAjedrez(FColor ColorA, FColor ColorB, int32 Celdas, int32 Resolucion) const
{
	UTexture2D* Textura = UTexture2D::CreateTransient(Resolucion, Resolucion, PF_B8G8R8A8);
	if (!Textura)
	{
		return nullptr;
	}

	Textura->SRGB = true;
	Textura->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	Textura->Filter = TextureFilter::TF_Nearest;

	const int32 TamCelda = FMath::Max(1, Resolucion / Celdas);
	FTexture2DMipMap& Mip = Textura->GetPlatformData()->Mips[0];
	FColor* Datos = static_cast<FColor*>(Mip.BulkData.Lock(LOCK_READ_WRITE));

	for (int32 Y = 0; Y < Resolucion; ++Y)
	{
		for (int32 X = 0; X < Resolucion; ++X)
		{
			const int32 Cx = X / TamCelda;
			const int32 Cy = Y / TamCelda;
			Datos[Y * Resolucion + X] = ((Cx + Cy) % 2 == 0) ? ColorA : ColorB;
		}
	}

	Mip.BulkData.Unlock();
	Textura->UpdateResource();
	return Textura;
}

UTexture2D* ARVPawn::CrearTexturaFranjas(FColor ColorA, FColor ColorB, int32 Franjas, int32 Resolucion) const
{
	UTexture2D* Textura = UTexture2D::CreateTransient(Resolucion, Resolucion, PF_B8G8R8A8);
	if (!Textura)
	{
		return nullptr;
	}

	Textura->SRGB = true;
	Textura->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	Textura->Filter = TextureFilter::TF_Nearest;

	const int32 AnchoBanda = FMath::Max(1, Resolucion / Franjas);
	FTexture2DMipMap& Mip = Textura->GetPlatformData()->Mips[0];
	FColor* Datos = static_cast<FColor*>(Mip.BulkData.Lock(LOCK_READ_WRITE));

	for (int32 Y = 0; Y < Resolucion; ++Y)
	{
		for (int32 X = 0; X < Resolucion; ++X)
		{
			const int32 Banda = (X + Y) / AnchoBanda;
			Datos[Y * Resolucion + X] = (Banda % 2 == 0) ? ColorA : ColorB;
		}
	}

	Mip.BulkData.Unlock();
	Textura->UpdateResource();
	return Textura;
}

UTexture2D* ARVPawn::CrearTexturaAnillos(FColor ColorA, FColor ColorB, int32 Anillos, int32 Resolucion) const
{
	UTexture2D* Textura = UTexture2D::CreateTransient(Resolucion, Resolucion, PF_B8G8R8A8);
	if (!Textura)
	{
		return nullptr;
	}

	Textura->SRGB = true;
	Textura->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	Textura->Filter = TextureFilter::TF_Nearest;

	const float Centro = Resolucion / 2.0f;
	const float Paso = Centro / static_cast<float>(Anillos);
	FTexture2DMipMap& Mip = Textura->GetPlatformData()->Mips[0];
	FColor* Datos = static_cast<FColor*>(Mip.BulkData.Lock(LOCK_READ_WRITE));

	for (int32 Y = 0; Y < Resolucion; ++Y)
	{
		for (int32 X = 0; X < Resolucion; ++X)
		{
			const float Dx = X - Centro;
			const float Dy = Y - Centro;
			const float D = FMath::Sqrt(Dx * Dx + Dy * Dy);
			const int32 Anillo = static_cast<int32>(D / Paso);
			Datos[Y * Resolucion + X] = (Anillo % 2 == 0) ? ColorA : ColorB;
		}
	}

	Mip.BulkData.Unlock();
	Textura->UpdateResource();
	return Textura;
}
