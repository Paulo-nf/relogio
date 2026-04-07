import random

def formatar_frames(lista_frames):
    itens = [f"{str(item):>2}" for item in lista_frames]
    return "[" + ", ".join(itens) + "]"


def simular_fifo(ref_string, num_frames):
    frames = []
    page_faults = 0
    total_acessos = len(ref_string)

    largura_coluna = max(10, (num_frames * 4))
    tracos_tabela = "-" * (15 + (largura_coluna * 2) + 10)

    print(f"\n--- Simulando FIFO com {num_frames} frames ---")
    print(f" Ref | {'Antes':<{largura_coluna}} | {'Depois':<{largura_coluna}} | Status")
    print(tracos_tabela)

    for pagina in ref_string:
        status = "Hit"
        frames_antes = frames.copy()

        if pagina not in frames:
            page_faults += 1
            status = "FAULT *"

            if len(frames) < num_frames:
                frames.append(pagina)
            else:
                frames.pop(0)
                frames.append(pagina)

        frames_antes_cheios = frames_antes + ['-'] * (num_frames - len(frames_antes))
        frames_depois_cheios = frames + ['-'] * (num_frames - len(frames))
        str_antes = formatar_frames(frames_antes_cheios)
        str_depois = formatar_frames(frames_depois_cheios)

        print(f" {pagina:2}  | {str_antes:<{largura_coluna}} | {str_depois:<{largura_coluna}} | {status}")

    taxa_pf = (page_faults / total_acessos) * 100
    print(tracos_tabela)
    print(f"Total de Page Faults (FIFO): {page_faults}")
    print(f"Taxa de Page Faults: {taxa_pf:.2f}%\n")


def simular_lru(ref_string, num_frames):
    frames = []
    page_faults = 0
    total_acessos = len(ref_string)

    largura_coluna = max(10, (num_frames * 4))
    tracos_tabela = "-" * (15 + (largura_coluna * 2) + 10)

    print(f"\n--- Simulando LRU com {num_frames} frames ---")
    print(f" Ref | {'Antes':<{largura_coluna}} | {'Depois':<{largura_coluna}} | Status")
    print(tracos_tabela)

    for pagina in ref_string:
        status = "Hit"
        frames_antes = frames.copy()

        if pagina in frames:
            frames.remove(pagina)
            frames.append(pagina)
        else:
            page_faults += 1
            status = "FAULT *"

            if len(frames) < num_frames:
                frames.append(pagina)
            else:
                frames.pop(0)
                frames.append(pagina)

        frames_antes_cheios = frames_antes + ['-'] * (num_frames - len(frames_antes))
        frames_depois_cheios = frames + ['-'] * (num_frames - len(frames))
        str_antes = formatar_frames(frames_antes_cheios)
        str_depois = formatar_frames(frames_depois_cheios)

        print(f" {pagina:2}  | {str_antes:<{largura_coluna}} | {str_depois:<{largura_coluna}} | {status}")

    taxa_pf = (page_faults / total_acessos) * 100
    print(tracos_tabela)
    print(f"Total de Page Faults (LRU): {page_faults}")
    print(f"Taxa de Page Faults: {taxa_pf:.2f}%\n")


def simular_clock(ref_string, num_frames):
    frames = []
    ref_bits = []
    ponteiro = 0
    page_faults = 0
    total_acessos = len(ref_string)

    largura_coluna = max(10, (num_frames * 4))
    tracos_tabela = "-" * (15 + (largura_coluna * 2) + 10)

    print(f"\n--- Simulando Clock com {num_frames} frames ---")
    print(f" Ref | {'Antes':<{largura_coluna}} | {'Depois':<{largura_coluna}} | Status")
    print(tracos_tabela)

    for pagina in ref_string:
        status = "Hit"
        frames_antes = frames.copy()

        if pagina in frames:
            idx = frames.index(pagina)
            ref_bits[idx] = 1
        else:
            page_faults += 1
            status = "FAULT *"

            if len(frames) < num_frames:
                frames.append(pagina)
                ref_bits.append(1)
            else:
                while True:
                    if ponteiro >= num_frames:
                        ponteiro = 0

                    if ref_bits[ponteiro] == 1:
                        ref_bits[ponteiro] = 0
                        ponteiro += 1
                    else:
                        frames[ponteiro] = pagina
                        ref_bits[ponteiro] = 1
                        ponteiro += 1
                        break

        frames_antes_cheios = frames_antes + ['-'] * (num_frames - len(frames_antes))
        frames_depois_cheios = frames + ['-'] * (num_frames - len(frames))
        str_antes = formatar_frames(frames_antes_cheios)
        str_depois = formatar_frames(frames_depois_cheios)

        print(f" {pagina:2}  | {str_antes:<{largura_coluna}} | {str_depois:<{largura_coluna}} | {status}")

    taxa_pf = (page_faults / total_acessos) * 100
    print(tracos_tabela)
    print(f"Total de Page Faults (Clock): {page_faults}")
    print(f"Taxa de Page Faults: {taxa_pf:.2f}%\n")


string_referencia = [random.randint(0, 16) for _ in range(16)]
qtd_frames = 8

print(f"String de Referência Gerada: {string_referencia}")

simular_fifo(string_referencia, qtd_frames)
simular_lru(string_referencia, qtd_frames)
simular_clock(string_referencia, qtd_frames)
