# Outrun

Este tutorial detalha a implementação do jogo "Outrun", um simulador de corrida com perspectiva 3D, desenvolvido em C++ e SFML. Abordaremos os conceitos fundamentais por trás da criação de uma estrada infinita e da projeção de objetos para simular um ambiente tridimensional.

## O que é Outrun

"Outrun" é um jogo de corrida clássico onde o jogador controla um carro em uma estrada que se estende ao horizonte, criando uma ilusão de profundidade e movimento. O objetivo é dirigir pela estrada, desviando de obstáculos e outros veículos, enquanto a paisagem se move para simular velocidade.

As principais características que exploraremos são:

-   **Perspectiva 3D:** Como a estrada e os objetos são renderizados para simular uma visão em três dimensões.
-   **Geração de Estrada:** A criação de uma estrada "infinita" com curvas e elevações.
-   **Controle de Veículo:** A movimentação do carro do jogador e o controle de velocidade.
-   **Objetos na Estrada:** A inclusão de elementos como árvores e carros oponentes.

## A Base do Jogo: Segmentos da Estrada e Projeção 3D

O coração do "Outrun" é a forma como ele simula um ambiente 3D usando segmentos de estrada e projeção. Em vez de um modelo 3D complexo, o jogo desenha a estrada como uma série de quadriláteros, cada um representando um pequeno segmento da pista.

### A Estrutura `Line`

Cada segmento da estrada é representado por uma instância da estrutura `Line`. Esta estrutura armazena as coordenadas 3D do centro do segmento (`x`, `y`, `z`), bem como suas propriedades na tela após a projeção (`X`, `Y`, `W`).

```cpp
struct Line {
	float x,y,z; // 3d center of line
	float X,Y,W; // screen coord
	float curve,spriteX,clip,scale;
	Sprite sprite;

	Line() {
		spriteX=curve=x=y=z=0;
	}

	void project(int camX,int camY,int camZ) {
		scale = camD/(z-camZ);
		X = (1 + scale*(x - camX)) * width/2;
		Y = (1 - scale*(y - camY)) * height/2;
		W = scale * roadW  * width/2;
	}

	void drawSprite(RenderWindow &app) {
		Sprite s = sprite;
		int w = s.getTextureRect().width;
		int h = s.getTextureRect().height;

		float destX = X + scale * spriteX * width/2;
		float destY = Y + 4;
		float destW  = w * W / 266;
		float destH  = h * W / 266;

		destX += destW * spriteX; // offsetX
		destY += destH * (-1);    // offsetY

		float clipH = destY+destH-clip;
		if (clipH<0) clipH=0;

		if (clipH>=destH) return;
		s.setTextureRect(IntRect(0,0,w,h-h*clipH/destH));
		s.setScale(destW/w,destH/h);
		s.setPosition(destX, destY);
		app.draw(s);
	}
};
```

-   **`x, y, z`**: Coordenadas 3D do centro do segmento. `z` é a profundidade, `y` a elevação e `x` a posição horizontal.
-   **`X, Y, W`**: Coordenadas na tela após a projeção. `X` é a posição horizontal, `Y` a vertical e `W` a largura do segmento na tela.
-   **`curve`**: Determina a curvatura do segmento da estrada. Um valor positivo curva para a direita, negativo para a esquerda.
-   **`spriteX`**: Posição horizontal de um sprite associado a este segmento (por exemplo, uma árvore ou um carro oponente).
-   **`project(camX, camY, camZ)`**: Este método é a chave para a projeção 3D. Ele calcula as coordenadas `X`, `Y` e `W` na tela com base na posição da câmera (`camX`, `camY`, `camZ`) e na profundidade (`z`) do segmento. Quanto maior `z` (mais distante), menor o `scale` e, consequentemente, menor o segmento na tela, criando a ilusão de profundidade.
-   **`drawSprite(RenderWindow &app)`**: Desenha um sprite associado a este segmento na posição correta na tela, levando em conta a perspectiva.

### Desenhando a Estrada: `drawQuad`

A função `drawQuad` é responsável por desenhar os quadriláteros que compõem a estrada. Ela recebe as coordenadas de dois pontos (superior e inferior) e suas respectivas larguras, e desenha um `ConvexShape` (um quadrilátero) entre eles.

```cpp
void drawQuad(RenderWindow &w, Color c, int x1,int y1,int w1,int x2,int y2,int w2) {
	ConvexShape shape(4);
	shape.setFillColor(c);
	shape.setPoint(0, Vector2f(x1-w1,y1));
	shape.setPoint(1, Vector2f(x2-w2,y2));
	shape.setPoint(2, Vector2f(x2+w2,y2));
	shape.setPoint(3, Vector2f(x1+w1,y1));
	w.draw(shape);
}
```

No loop principal do jogo, esta função é chamada repetidamente para desenhar a grama, o acostamento e a pista, com cores diferentes para cada parte.

## O Loop Principal do Jogo

O `main` function contém o loop principal do jogo, onde a lógica é atualizada e a tela é redesenhada a cada frame.

```cpp
int main() {
	RenderWindow app(VideoMode(width, height), "Outrun Racing!");
	app.setFramerateLimit(60);

	// Carregamento de Texturas e Sprites
	Texture t[50];
	Sprite object[50];
	for(int i=1; i<=7; i++) {
		t[i].loadFromFile("images/" + std::to_string(i) + ".png");
		t[i].setSmooth(true);
		object[i].setTexture(t[i]);
	}

	Texture bg;
	bg.loadFromFile("images/bg.png");
	bg.setRepeated(true);
	Sprite sBackground(bg);
	sBackground.setTextureRect(IntRect(0,0,5000,411));
	sBackground.setPosition(-2000,0);

	// Geração da Estrada (linhas)
	std::vector<Line> lines;

	for(int i=0; i<1600; i++) {
		Line line;
		line.z = i*segL;

		if (i>300 && i<700) line.curve=0.5;
		if (i>1100) line.curve=-0.7;

		// Adição de Sprites (árvores, carros oponentes, etc.)
		if (i<300 && i%20==0) {
			line.spriteX=-2.5;
			line.sprite=object[5];
		}
		if (i%17==0)          {
			line.spriteX=2.0;
			line.sprite=object[6];
		}
		if (i>300 && i%20==0) {
			line.spriteX=-0.7;
			line.sprite=object[4];
		}
		if (i>800 && i%20==0) {
			line.spriteX=-1.2;
			line.sprite=object[1];
		}
		if (i==400)           {
			line.spriteX=-1.2;
			line.sprite=object[7];
		}
		// Carros oponentes adicionados
		if (i>100 && i%100==0) {
			line.spriteX=0.5;
			line.sprite=object[1];
		}
		if (i>200 && i%150==0) {
			line.spriteX=-0.5;
			line.sprite=object[1];
		}

		if (i>750) line.y = sin(i/30.0)*1500; // Elevação da estrada

		lines.push_back(line);
	}

	int N = static_cast<int>(lines.size());
	float playerX = 0;
	int pos = 0;
	int H = 1500; // Altura da câmera

	while (app.isOpen()) {
		Event e;
		while (app.pollEvent(e)) {
			if (e.type == Event::Closed)
				app.close();
		}

		int speed=0;

		// Input do Jogador
		if (Keyboard::isKeyPressed(Keyboard::Right)) playerX+=0.1;
		if (Keyboard::isKeyPressed(Keyboard::Left)) playerX-=0.1;
		if (Keyboard::isKeyPressed(Keyboard::Up)) speed=200;
		if (Keyboard::isKeyPressed(Keyboard::Down)) speed=-200;
		if (Keyboard::isKeyPressed(Keyboard::Tab)) speed*=3;

		// Atualização da Posição na Estrada
		pos+=speed;
		while (pos >= N*segL) pos-=N*segL;
		while (pos < 0) pos += N*segL;

		app.clear(Color(105,205,4));
		app.draw(sBackground);

		int startPos = pos/segL;
		int camH = lines[startPos].y + H; // Altura da câmera ajustada pela elevação da estrada
		if (speed>0) sBackground.move(-lines[startPos].curve*2,0);
		if (speed<0) sBackground.move( lines[startPos].curve*2,0);

		int maxy = height;
		float x=0,dx=0;

		/////// Desenho da Estrada ////////
		for(int n = startPos; n < startPos+300; n++) {
			Line &l = lines[n%N];
			l.project(playerX*roadW - x, camH, pos - (n >= N ? N*segL : 0));
			x+=dx;
			dx+=l.curve;

			l.clip=maxy;
			if (l.Y>=maxy) continue;
			maxy = l.Y;

			Color grass  = (n/3)%2?Color(16,200,16):Color(0,154,0);
			Color rumble = (n/3)%2?Color(255,255,255):Color(0,0,0);
			Color road   = (n/3)%2?Color(107,107,107):Color(105,105,105);

			Line p = lines[(n-1+N)%N]; // previous line

			drawQuad(app, grass, 0, p.Y, width, 0, l.Y, width);
			drawQuad(app, rumble,p.X, p.Y, p.W*1.2, l.X, l.Y, l.W*1.2);
			drawQuad(app, road,  p.X, p.Y, p.W, l.X, l.Y, l.W);
		}

		//////// Desenho de Objetos ////////
		for(int n=startPos+300; n>startPos; n--)
			lines[n%N].drawSprite(app);

		app.display();
	}

	return 0;
}
```

### Geração da Estrada

A estrada é gerada como um vetor de objetos `Line`. Cada `Line` tem uma posição `z` (profundidade) e pode ter uma `curve` (curvatura) e `y` (elevação) para criar um percurso variado. Sprites (árvores, carros oponentes) são adicionados a segmentos específicos da estrada.

### Input do Jogador

O jogo responde às teclas de seta para controlar a posição horizontal do jogador (`playerX`) e a velocidade (`speed`). A tecla `Tab` atua como um "boost" de velocidade.

### Atualização da Posição

A variável `pos` representa a posição atual do jogador na estrada. Ela é incrementada pela `speed` a cada frame. O uso do operador `%` (`pos % (N*segL)`) garante que a estrada seja "infinita", reciclando os segmentos quando o jogador atinge o final.

### Desenho da Estrada e Objetos

O loop de desenho itera sobre os segmentos da estrada visíveis. Para cada segmento, ele:

1.  **Projeta** o segmento para as coordenadas da tela usando `l.project()`.
2.  **Calcula** a curvatura acumulada (`x` e `dx`) para simular a perspectiva da estrada.
3.  **Desenha** a grama, o acostamento e a pista usando `drawQuad`, com cores alternadas para criar um efeito de faixas.
4.  **Desenha os sprites** associados a cada segmento (árvores, carros oponentes) usando `lines[n%N].drawSprite(app)`.

O desenho dos objetos é feito em ordem inversa (`n=startPos+300; n>startPos; n--`) para garantir que os objetos mais distantes sejam desenhados primeiro, e os mais próximos por cima, mantendo a ordem de profundidade correta.

## Carros Oponentes

Os carros oponentes são implementados como sprites associados a segmentos específicos da estrada. Eles são adicionados durante a fase de geração da estrada:

```cpp
		// Carros oponentes adicionados
		if (i>100 && i%100==0) { // Adiciona um carro a cada 100 segmentos após os primeiros 100
			line.spriteX=0.5; // Posição horizontal do carro
			line.sprite=object[1]; // Sprite do carro (placeholder)
		}
		if (i>200 && i%150==0) { // Adiciona outro carro a cada 150 segmentos após os primeiros 200
			line.spriteX=-0.5; // Posição horizontal do carro
			line.sprite=object[1]; // Sprite do carro (placeholder)
		}
```

Atualmente, eles usam `object[1]` como um sprite placeholder e são posicionados em diferentes faixas da estrada usando `spriteX`.

## Extensões Possíveis

Este jogo serve como uma excelente base para futuras extensões:

-   **Detecção de Colisão:** Implementar a lógica para detectar quando o carro do jogador colide com os carros oponentes ou outros objetos na estrada.
-   **Sistema de Pontuação:** Adicionar um sistema de pontuação baseado na distância percorrida, velocidade ou desvio de obstáculos.
-   **Tipos de Oponentes Variados:** Introduzir diferentes tipos de carros oponentes com comportamentos distintos.
-   **Múltiplas Pistas/Cenários:** Criar diferentes ambientes e layouts de estrada para aumentar a variedade.
-   **Efeitos Sonoros:** Adicionar sons para o motor, colisões e música de fundo.
-   **Interface de Usuário:** Exibir informações como velocidade, pontuação e distância percorrida.

Este tutorial cobriu os aspectos fundamentais da implementação do jogo "Outrun", desde a projeção 3D da estrada até a inclusão de objetos e o controle do jogador. Com esta base, é possível expandir o jogo com diversas funcionalidades para torná-lo mais rico e interativo.