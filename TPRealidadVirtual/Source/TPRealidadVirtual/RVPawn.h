#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RVPawn.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UCameraComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UTexture2D;

UCLASS()
class TPREALIDADVIRTUAL_API ARVPawn : public APawn
{
	GENERATED_BODY()

public:
	ARVPawn();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RaizEscena;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RaizGema;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ConoSuperior;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ConoInferior;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* PlanosPivote;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PlanoA;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PlanoB;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camara;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* MaterialBasePatron;

	UPROPERTY()
	UMaterialInstanceDynamic* MaterialDinamicoA;

	UPROPERTY()
	UMaterialInstanceDynamic* MaterialDinamicoB;

	FVector EjeRotacion;
	float VelocidadActual;
	float VelocidadBase;
	int32 ContadorPatron;

	void GirarArriba();
	void GirarAbajo();
	void GirarIzquierda();
	void GirarDerecha();
	void Detener();
	void Reiniciar();
	void AplicarPatron();

	UTexture2D* CrearTexturaAjedrez(FColor ColorA, FColor ColorB, int32 Celdas, int32 Resolucion) const;
	UTexture2D* CrearTexturaFranjas(FColor ColorA, FColor ColorB, int32 Franjas, int32 Resolucion) const;
	UTexture2D* CrearTexturaAnillos(FColor ColorA, FColor ColorB, int32 Anillos, int32 Resolucion) const;
};
