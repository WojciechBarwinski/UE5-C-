#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"
#include "HUD/HealthBarComponent.h"

void UHealthBarComponent::SetHealthPercent(float Percent)
{
    if (HealthBarWidget == nullptr)
    {
        HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
    }

    if (HealthBarWidget && HealthBarWidget->HealthBar)
    {
        HealthBarWidget->HealthBar->SetPercent(Percent);
    }
}
