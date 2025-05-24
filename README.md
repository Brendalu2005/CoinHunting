# 🪙 CoinHunting

## 📖 Um pouco de CoinHunting:
**CoinHunting** é um jogo em visão **top-down**, onde o objetivo principal é coletar o máximo de moedas possível dentro de cavernas misteriosas. O jogador pode escolher entre dois modos: **1 jogador** ou **2 jogadores**.

---
<details>
<summary> 🚀 COMO INSTALAR </summary>

## 🚀 Instale a Raylib no Linux (Zorin OS / Ubuntu)
Este guia rápido mostra como instalar a biblioteca **Raylib** em distros Linux baseadas no Ubuntu, como o **Zorin OS**. .

---

## ✅ Passo a passo

### 1. Atualize os pacotes do sistema

Abra o terminal(<kbd>Ctrl + Alt + T</kbd>)e execute:

```
sudo apt update && sudo apt upgrade
```
OBS: caso não tenha instalado o make/gcc/git:
```
 sudo apt install build-essential git
```
### 2. Instalar as dependencias e tudo que a raylib precisa para compilar e rodar corretamente
```
sudo apt install build-essential git cmake libasound2-dev libpulse-dev libx11-dev libxcursor-dev libxinerama-dev libxrandr-dev libxi-dev libgl1-mesa-dev
```
### 3. Baixar o repositorio da raylib no computador
```
git clone https://github.com/raysan5/raylib.git
```
### 4. Agora vamos entrar na pasta que foi criada:
```
cd raylib
```
### 5. Vamos criar uma pasta e depois entrar nela:
```
mkdir build && cd build
```
### 6. Gere os arquivos de compilação com CMake:
```
cmake ..
```
### 7. Compile a Raylib:
```
make
```
### 8. Por fim, instale a biblioteca no sistema:
```
sudo make install
```

---

## 🪙Como rodar o CoinHunting?

### 1. Clone o repositório do jogo:
```
git clone https://github.com/Brendalu2005/CoinHunting.git
```
ou 
```
git clone git@github.com:Brendalu2005/CoinHunting.git
```
### 2. Entre na pasta do jogo:
```
cd CoinHunting
```
### 3. compile e depois rode o jogo:
```
make
```
depois
```
./main
```
</details>

---

<details>
<summary>🎮 SOBRE O JOGO</summary>

## 🎮 Modos de Jogo

### 👤 1 Jogador
- Capture o maior número de moedas antes que o tempo acabe.
- Desvie dos fantasmas que tentarão atrapalhar sua missão.
- Ao fim da partida, insira um **Nickname**.
- Se sua pontuação estiver entre as 10 maiores, ela aparecerá no **Ranking**.

### 👥 2 Jogadores
- Dois jogadores competem para ver quem coleta mais moedas.
- **Player 1** usa as teclas **W, A, S, D**.
- **Player 2** usa as **setas direcionais**.
- No final, o jogador vencedor insere um **Nickname**.
- Se ele estiver no  **top 10**, será exibido no **Ranking**.

---

## 📋 Menu Principal

O menu inicial possui três opções:

- **Iniciar** ->Escolha entre jogar com 1 ou 2 jogadores.  
- **Sobre** -> Informações sobre o jogo e como jogar.  
- **Ranking** -> Exibe os 10 melhores jogadores.

---

## 🎮 Controles

### Jogador 1  
- **W** → Mover para cima  
- **A** → Mover para esquerda  
- **S** → Mover para baixo  
- **D** → Mover para direita  

### Jogador 2  
- **⬆️** → Mover para cima  
- **⬅️** → Mover para esquerda  
- **⬇️** → Mover para baixo  
- **➡️** → Mover para direita  

---

## 🏆 Ranking

Os 5 melhores jogadores, com as maiores pontuações são exibidos no menu **Ranking**, junto com seus apelidos e número de moedas coletadas.


</details>

---

<details>
<summary> 🛠️TECNOLOGIAS UTILIZADAS</summary>

## 🛠️ Tecnologias e Ferramentas Utilizadas

Este jogo foi desenvolvido utilizando a linguagem **C**, aprendida na disciplina de **PIF (Programação Imperativa e Funcional)**. Além da linguagem **C**, utilizamos a biblioteca:

- 🎮 [**Raylib**](https://www.raylib.com/)  
  Optamos pela raylib pois ela é voltada para o desenvolvimento de jogos 2D e 3D, oferecendo suporte a sprites, imagens, sons e outros elementos gráficos e sonoros essenciais.Além disso a comunidade é bastante ativa, facilitanto achar tutoriais e documentação.

</details>

---

<details>
<summary>🎬VIDEO DEMONSTRATIVO</summary>

[**Video demonstrando a jogabilidade**](https://youtu.be/GeS_lryc51Q)

</details>

---

<details>
<summary>📋SLIDE</summary>

[**SLIDE DO COINHUNTING**](https://www.canva.com/design/DAGoR4tnGrs/RfzboSHJ40W7m_BQd07BnQ/edit?utm_content=DAGoR4tnGrs&utm_campaign=designshare&utm_medium=link2&utm_source=sharebutton)

</details>

---



## 🫂 Equipe de desenvolvimento
| Nome                                  | Email da school    |
| ------------------------------------- | ------------------ | 
| [**Augusto Malheiros de Souza**](https://github.com/goodguto)            | ams10@cesar.school | 
| [**Brenda Luana Correia Bezerra**](https://github.com/Brendalu2005)          | blcb@cesar.school  |
| [**Eduardo Albuquerque Alves Barbosa**](https://github.com/eduaab)     | eaab@cesar.school  |

---
