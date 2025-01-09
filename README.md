## 언리얼 포트폴리오 제작

해당 프로젝트를 실행하려면 마켓플레이스에서 **Paragon Countess, Paragon Minions**를 프로젝트에 추가해야 합니다.   
<br/>
아래 블로그 링크에서 코드 및 구현 일지를 볼 수 있습니다   
<br/>
<a href="https://k99812.tistory.com/category/%ED%8F%AC%ED%8A%B8%ED%8F%B4%EB%A6%AC%EC%98%A4%20%EC%A0%9C%EC%9E%91" height="5" width="10" target="_blank" >
<img src="https://img.shields.io/badge/블로그링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white">
</a>

<br/>

## 기술 설명서

## Character 구조
![image](https://github.com/user-attachments/assets/e1efdf56-1f39-42af-a700-45ef463b2cf9)


### CharacterBase
플레이어 캐릭터, NPC 캐릭터가 공통적으로 사용하는 함수 및 변수 선언
* Dead 이벤트 관련 함수 및 몽타주
* 콤보공격 관련 변수 및 몽타주
* 캐릭터의 소속을 가진 게임 플레이 태그 관리


### PlayerCharacter
캐릭터의 스탯을 관리하는 어트리뷰트셋은 플레이어 스테이트 클래스에 선언
* 캐릭터의 능력 및 스탯을 관리 해주는 ASC를 생성
* Dead 관련 함수들을 Override
* 사용자 입력처리
* GA 생성 
* 어트리뷰트셋 적용


### NonPlayerCharacter
추후 보스몬스터를 추가할 수 있어 보스몬스터, 잡몹의 공통 함수 및 변수를 선언
* 능력, 스탯 관리 ASC 생성
* 스탯을 저장할 어트리뷰트셋 생성
* Dead 관련 함수 Override

### CharacterGrunt
잡몹 클래스
* 체력 HPBar 위젯 관리
* 어트리뷰트 초기화

## 입력 처리
![image](https://github.com/user-attachments/assets/8c7fb823-4e61-4d59-81b2-43ccdae2e110)

EnhancedInput을 사용
InputMappintContext에서 InputAction을 맵핑 후 저장

## GA(능력) 처리
GA의 부여는 캐릭터가 빙의될때 호출되는 PossessedBy 함수에서 진행됨
'''
UPROPERTY(EditAnywhere, Category = "GAS")
TArray<TSubclassOf<class UGameplayAbility>> StartAbilites;

for (const TSubclassOf<UGameplayAbility>& StartAbility : StartAbilites)
{
	FGameplayAbilitySpec Spec(StartAbility);
	ASC->GiveAbility(Spec);
}
'''
실행에 입력이 필요없는 GA(Ex 공격판정)들은 TArray를 사용해 블루프린트 클래스를 통해 TArray에 지정
![image](https://github.com/user-attachments/assets/2c939ffe-2e9b-4c8e-873f-ce75342d2506)


'''C++
UENUM(BlueprintType)
enum class EInputAbility : uint8
{
	None UMETA(DisplayName = "None"),
	Jump = 10 UMETA(DisplayName = "Jump"),
	Sprint UMETA(DisplayName = "Sprint"),
	LeftAttack UMETA(DisplayName = "LAttack"),
	RightAttack UMETA(DisplayName = "RAttack"),
	Skill UMETA(DisplayName = "Skill")
};
'''

'''
UPROPERTY(EditAnywhere, Category = "GAS")
TMap<EInputAbility, TSubclassOf<class UGameplayAbility>> StartInputAbilites;

for (const TPair<EInputAbility, TSubclassOf<class UGameplayAbility>>& StartInputAbility : StartInputAbilites)
{
	FGameplayAbilitySpec Spec(StartInputAbility.Value);

	Spec.InputID = (int32)StartInputAbility.Key;

	ASC->GiveAbility(Spec);
}
'''
입력을 통해서 발동되는 GA들은 TMap으로 <열겨형, GA>로 받아 Key값을 GA를 부여할때 InputID로 지정
열거형을 사용한 이유는 int로 지정시 지정한 숫자가 맞는지 코드를 통해서 확인 해야됨 
열거형은 코드를 이름을 통해서 지정을 하여 직관적이라고 느껴 열거형을 사용함 
또 별도의 헤더파일을 생성하여 선언해 다른 클래스에서 GA를 사용할때 해당 열거형을 추가하여 사용 가능

![image](https://github.com/user-attachments/assets/94a63822-dcae-474e-a31a-ca5538c30a11)


<br/>

## Controller
### PlayerController
* HUD 관리
* 인풋모드 관리


### AIController
![image](https://github.com/user-attachments/assets/37c90fd0-c08d-4018-9a61-3e35d7d1be04)

AI컨트롤러에서 AI Perception 기능을 이용해 적을 인식 하여 블랙보드에 저장합니다
AI가 적을 인식할때 델리게이트를 이용하여 몬스터의 HPBar를 컨트롤 합니다   <a href="https://k99812.tistory.com/123" height="5" width="10" target="_blank" ><img src="https://img.shields.io/badge/코드링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white"></a>
* AISense 관리
* 블랙보드, 행동트리 관리
* AIPerception 이벤트 처리

### 행동트리
![image](https://github.com/user-attachments/assets/92f1224a-b851-48a2-9c5f-eff7578e503a)



## UI
![image](https://github.com/user-attachments/assets/3b7bde60-6a59-44ea-86c5-f2f5b3741e28)
<a href="https://k99812.tistory.com/119" height="5" width="10" target="_blank" ><img src="https://img.shields.io/badge/코드링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white"></a>

### WidgetComponent
* InitWidget 함수에서 생성한 위젯을 가져와 오너를 넘겨줌

### PPGASUserWidget
위젯 컴포넌트를 이용하여 위젯을 생성하면 GetOwningPlayer 함수를 사용할 수 없음 
그래서 별도로 위젯 컴포넌트에서 오너를 받아올 수 있는 함수를 생성해야 됨
* SetAbilitySystemComponent 가상함수를 선언하여 WidgetComponent에서 오너를 받음

### PPGASHPBarUserWidget
* SetAbilitySystemComponent 함수에서 매개변수로 들어온 오너를 이용하여 ASC에 어트리뷰트 체인지 델리게이트에 함수 등록
* 프로그래스바, 텍스트 박스 관리

## Player HUD
![image](https://github.com/user-attachments/assets/e31c1aba-8d31-4b98-bb7d-23700a240a32)
<a href="https://k99812.tistory.com/126" height="5" width="10" target="_blank" ><img src="https://img.shields.io/badge/코드링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white"></a>

### PPHUDWidget
* 생성한 위젯들을 관리할 클래스

### PPPlayerStatBarUserWidget
* ASC를 통해 어트리뷰트 체인지 델리게이트를 통해 콜백함수 연결
* 프로그래스바, 텍스트박스 관리


<br/>

## 진행상황
1. 캐릭터 이동 및 애니메이션
2. GAS를 이용해 공격, 점프, 달리기 구현
3. AttributeSet을 이용한 스탯 구현
4. 공격 판정 및 데미지 구현
5. 잡몹 HPBar, 체력 0 될시 죽는 몽타주 실행 및 Destroy
6. 행동트리를 이용해 몬스터 AI 구현

## 캐릭터 이동
https://github.com/user-attachments/assets/d468b24e-221d-41f4-9882-67265ac3ae82

<br/>

## 점프, 달리기, 공격
https://github.com/user-attachments/assets/70242ec3-3067-40f8-95a6-00b82769add8

<br/>

## 공격 판정, 몬스터 체력 감소
https://github.com/user-attachments/assets/bac2ceb7-010f-45d1-8f28-6b4412a1e1af

<br/>
<div align="center">
  <img src="https://github.com/user-attachments/assets/18b56d25-2973-482a-85bd-7e7d1ae492ed" width="150" height="200" title="px(픽셀) 크기 설정" alt="Attribute"></img>
  <img src="https://github.com/user-attachments/assets/fb611ded-5cc0-442d-a126-93809ab896df" width="150" height="200" title="px(픽셀) 크기 설정" alt="Attribute"></img>
</div>

<br/>

## 체력바, 0이 될시 죽는 몽타주 실행
https://github.com/user-attachments/assets/6e2dce8e-3aff-4ead-a0e2-844e4ce36ef4

<br/>

## 몬스터 AI
https://github.com/user-attachments/assets/2d78aa19-0131-4055-b490-936a8b35c4de

초록색 범위 : 몬스터 시야 인식 범위
빨강색 범위 : 몬스터가 인식한 경우 해당 범위를 벗어나야 잊혀짐
노랑색 범위 : 노이즈 이벤트 범위


https://github.com/user-attachments/assets/304a73f8-e93c-4e49-8669-28b2bcbe6248


<br/>


