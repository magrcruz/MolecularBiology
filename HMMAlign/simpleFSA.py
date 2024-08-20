def score_function(xi, yj):
    return 1 if xi == yj else -1

# Penalización por gap
d = 4

# Penalización por gaps consecutivos
e = 4

def maxDir(*args):
    max_value = float('-inf')
    origin = None

    for i, value in enumerate(args):
        if value[0] > max_value:#asumiendo que esta en el par valor origen
            max_value = value[0]
            origin = value[1]

    return max_value, origin

def dynamic_programming_alignment_with_directions(x, y, d, e):
    # Longitudes de las secuencias
    m = len(x)
    n = len(y)

    # Inicialización de las matrices y las matrices de direcciones
    V_M = [[0 for _ in range(n + 1)] for _ in range(m + 1)]
    V_X = [[0 for _ in range(n + 1)] for _ in range(m + 1)]
    V_Y = [[0 for _ in range(n + 1)] for _ in range(m + 1)]

    # Direcciones: 0 para diagonal, 1 para arriba, 2 para izquierda
    direction_M = [[0 for _ in range(n + 1)] for _ in range(m + 1)]
    direction_X = [[0 for _ in range(n + 1)] for _ in range(m + 1)]
    direction_Y = [[0 for _ in range(n + 1)] for _ in range(m + 1)]

    # Inicializaciones
    for i in range(1, m + 1):
        V_M[i][0] = float('-inf')
        V_X[i][0] = float('-inf')
        V_Y[i][0] = float('-inf')
    
    for j in range(1, n + 1):
        V_M[0][j] = float('-inf')
        V_X[0][j] = float('-inf')
        V_Y[0][j] = float('-inf')

    # Llenado de las matrices y las matrices de direcciones
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            score_M = score_function(x[i - 1], y[j - 1])
            V_M[i][j] = score_M + max(V_M[i - 1][j - 1], V_X[i - 1][j - 1], V_Y[i - 1][j - 1])
            direction_M[i][j] = [i - 1, j - 1]  # Dirección: diagonal

            V_X[i][j], direction_X[i][j] = maxDir(
                [V_M[i - 1][j] - d, [i-1,j]],
                [V_X[i - 1][j] - e, [i,j-1]]
            )

            V_Y[i][j], direction_Y[i][j] = maxDir(
                [V_M[i][j - 1] - d, [i,j-1]],
                [V_Y[i][j - 1] - e, [i-1,j]]
            )
            # Dirección: izquierda si proviene de V_M, arriba si proviene de V_Y

    return V_M, V_X, V_Y, direction_M, direction_X, direction_Y


# Ejemplo de uso
x = "AGTACGCA"
y = "TATGC"
d = 2
e = 1

result_matrices = dynamic_programming_alignment_with_directions(x, y, d, e)
V_M, V_X, V_Y, direction_M, direction_X, direction_Y = result_matrices

# Puedes imprimir las matrices de direcciones para ver de dónde provienen los valores
print("Matriz de direcciones (V_M):")
for row in direction_M:
    print(row)

print("\nMatriz de direcciones (V_X):")
for row in direction_X:
    print(row)

print("\nMatriz de direcciones (V_Y):")
for row in direction_Y:
    print(row)

def traceback_with_directions(direction_M, direction_X, direction_Y, x, y, i, j, aligned_x, aligned_y):
    print("iter")
    if i == 0 and j == 0:
        return [''.join(aligned_x[::-1]), ''.join(aligned_y[::-1])]

    if i == 0:
        return traceback_with_directions(direction_M, direction_X, direction_Y, x, y, i, j - 1, aligned_x + ['-'], aligned_y + [y[j - 1]])

    if j == 0:
        return traceback_with_directions(direction_M, direction_X, direction_Y, x, y, i - 1, j, aligned_x + [x[i - 1]], aligned_y + ['-'])


    if direction_M[i][j] == [i - 1, j - 1]:
        return traceback_with_directions(direction_M, direction_X, direction_Y, x, y, i - 1, j - 1, aligned_x + [x[i - 1]], aligned_y + [y[j - 1]])
    elif direction_X[i][j] == [i - 1, j]:
        return traceback_with_directions(direction_M, direction_X, direction_Y, x, y, i - 1, j, aligned_x + [x[i - 1]], aligned_y + ['-'])
    elif direction_Y[i][j] == [i, j - 1]:
        return traceback_with_directions(direction_M, direction_X, direction_Y, x, y, i, j - 1, aligned_x + ['-'], aligned_y + [y[j - 1]])


aligned_sequences = traceback_with_directions(direction_M, direction_X, direction_Y, x, y, len(x), len(y), [], [])

# Imprime el resultado del alineamiento
print("Alineamiento x:", aligned_sequences[0])
print("Alineamiento y:", aligned_sequences[1])
