import serial
import serial.tools.list_ports
import matplotlib.pyplot as plt


# ===== LISTA PORTAS DISPONÍVEIS =====

portas = list(serial.tools.list_ports.comports())

if len(portas) == 0:
    print("Nenhuma porta serial encontrada.")
    exit()

print("Portas disponíveis:\n")

for i, porta in enumerate(portas):
    print(f"{i} - {porta.device} - {porta.description}")


# ===== USUÁRIO ESCOLHE A PORTA =====

while True:

    try:
        escolha = int(
            input("\nDigite o número da porta que deseja usar: ")
        )

        if escolha < 0 or escolha >= len(portas):
            print("Opção inválida.")
            continue

        porta_escolhida = portas[escolha].device

        break

    except ValueError:
        print("Digite apenas o número da porta.")


# ===== CONEXÃO SERIAL =====

try:

    ser = serial.Serial(
        porta_escolhida,
        9600,
        timeout=1
    )

    print("\nConectado em:", ser.port)

except Exception as erro:

    print("Erro ao conectar na porta:")
    print(erro)

    exit()


# ===== CONFIG DO GRÁFICO =====

frequencias = [
    '415',
    '445',
    '480',
    '515',
    '555',
    '590',
    '630',
    '680'
]

cores = [
    '#8A2BE2',
    '#0000FF',
    '#00BFFF',
    '#00FF00',
    '#FFFF00',
    '#FFD700',
    '#FF4500',
    '#FF0000'
]


plt.ion()

fig, ax = plt.subplots()


# ===== LOOP PRINCIPAL =====

while True:

    vetor = []

    while len(vetor) < 9:

        try:

            linha = ser.readline().decode(
                'utf-8'
            ).strip()


            if linha == "":
                continue


            valor = float(linha)

            vetor.append(valor)


        # Se não for número,
        # mostra a mensagem recebida do Arduino
        except ValueError:

            print(linha)

            continue


        except Exception as erro:

            print("Erro:", erro)

            continue


    intensidades = vetor[:8]

    comprimento_dominante = vetor[8]


    # ===== PLOT =====

    ax.clear()


    ax.bar(
        frequencias,
        intensidades,
        color=cores
    )


    ax.set_xlabel(
        "Comprimento de onda (nm)"
    )

    ax.set_ylabel(
        "Intensidade"
    )

    ax.set_title(
        "Leitura espectral - AS7341"
    )


    ax.text(
        0.95,
        0.95,
        f'Dominante: {comprimento_dominante:.1f} nm',
        transform=ax.transAxes,
        ha='right',
        va='top',
        bbox=dict(
            facecolor='white',
            alpha=0.7
        )
    )


    plt.pause(0.1)


    if not plt.fignum_exists(fig.number):

        print("Encerrado")

        break


ser.close()