# Algoritmo de viterbi

def getMatrix(x, y, p, q, delta, epsilon, tau):
    n = len(x)
    m = len(y)

    vM = [[0.0] * (m + 1) for i in range(n + 1)]
    vX = [[0.0] * (m + 1) for i in range(n + 1)]
    vY = [[0.0] * (m + 1) for i in range(n + 1)]

    vM[0][0] = 1.0

    # Relaciones de recurrencia
    for i in range(1, n + 1):
        for j in range(1, m + 1):
            vM[i][j] = p[x[i - 1]][y[j - 1]] * max(
                (1 - 2 * delta - tau) * vM[i - 1][j - 1],
                (1 - epsilon - tau) * vX[i - 1][j - 1],
                (1 - epsilon - tau) * vY[i - 1][j - 1]
            )

        if i > 1:
            for j in range(m + 1):
                vX[i][j] = q[x[i - 1]] * max(
                    delta * vM[i - 1][j],
                    epsilon * vX[i - 1][j]
                )

        if j > 1:
            for i in range(n + 1):
                vY[i][j] = q[y[j - 1]] * max(
                    delta * vM[i][j - 1],
                    epsilon * vY[i][j - 1]
                )

    vE = tau * max(vM[n][m], vX[n][m], vY[n][m])
    return vM, vX, vY

def getSequences(x, y, p, q, vM, vX, vY):
    # Backtrack para obtener el alineamiento
    i, j = len(x), len(y)
    seqx = ""
    seqy = ""

    while i > 0 or j > 0:
        if i > 0 and j > 0 and vM[i][j] == p[x[i - 1]][y[j - 1]] * vM[i - 1][j - 1]:
            #Vino de un estado de match
            seqx = x[i - 1] + seqx
            seqy = y[j - 1] + seqy
            i -= 1
            j -= 1
        elif i > 0 and vX[i][j] == q[x[i - 1]] * vM[i - 1][j]:
            #Vino de un estado X
            seqx = x[i - 1] + seqx
            seqy = "-" + seqy
            i -= 1
        else:
            #Vino de un estado Y
            seqx = "-" + seqx
            seqy = y[j - 1] + seqy
            j -= 1

    return seqx, seqy

def viterbi(x, y):
    delta = 0.01
    epsilon = 0.01
    tau = 0.01

    q = {'A': 0.3, 'C': 0.2, 'G': 0.2, 'T': 0.3} 
    p = {'A': {'A': 0.1, 'C': 0.4, 'G': 0.2, 'T': 0.3},
        'C': {'A': 0.2, 'C': 0.3, 'G': 0.1, 'T': 0.4},
        'G': {'A': 0.4, 'C': 0.1, 'G': 0.3, 'T': 0.2},
        'T': {'A': 0.3, 'C': 0.2, 'G': 0.4, 'T': 0.1}}

    vM, vX, vY = getMatrix(x, y, p, q, delta, epsilon, tau)
    seqx, seqy = getSequences(x, y, p, q, vM, vX, vY)

    print("Secuencias")
    print("X:", x)
    print("Y:", y)

    print("Alineamientos")
    print("A:", seqx)
    print("B:", seqy)
    print()
    return seqx, seqy

seqx, seqy = viterbi("AGT", "ACGT")

