## 언리얼 포트폴리어 제작

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
* 캐릭터 구현  <a href="https://k99812.tistory.com/85" height="5" width="10" target="_blank" ><img src="https://img.shields.io/badge/코드링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white"></a>
* 인풋 구현 <a href="https://k99812.tistory.com/86" height="5" width="10" target="_blank" ><img src="https://img.shields.io/badge/코드링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white"></a>
* 애니메이션 구현 <a href="https://k99812.tistory.com/93" height="5" width="10" target="_blank" ><img src="https://img.shields.io/badge/코드링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white"></a>
* GA 생성 <a href="https://k99812.tistory.com/101" height="5" width="10" target="_blank" ><img src="https://img.shields.io/badge/코드링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white"></a>
* 입력에 따른 GA 실행 <a href="https://k99812.tistory.com/102" height="5" width="10" target="_blank" ><img src="https://img.shields.io/badge/코드링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white"></a>
* 어트리뷰트셋 생성 <a href="https://k99812.tistory.com/116" height="5" width="10" target="_blank" ><img src="https://img.shields.io/badge/코드링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white"></a>


### NonPlayerCharacter
추후 보스몬스터를 추가할 수 있어 보스몬스터, 잡몹의 공통 함수 및 변수를 선언
* 능력, 스탯 관리 ASC 생성
* 스탯을 저장할 어트리뷰트셋 관리
* Dead 관련 함수 Override

### CharacterGrunt
* 체력 HPBar UI 관리

## Controller
### PlayerController
* HUD 관리
* 인풋모드 관리


### AIController
![image](https://github.com/user-attachments/assets/37c90fd0-c08d-4018-9a61-3e35d7d1be04)

AI컨트롤러에서 AI Perception 기능을 이용해 적을 인식 하여 블랙보드에 저장합니다
AI가 적을 인식할때 델리게이트를 이용하여 몬스터의 HPBar를 컨트롤 합니다   <a href="https://k99812.tistory.com/123" height="5" width="10" target="_blank" ><img src="https://img.shields.io/badge/코드링크-E4501E?style=for-the-badge&logo=Tistory&logoColor=white"></a>

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

