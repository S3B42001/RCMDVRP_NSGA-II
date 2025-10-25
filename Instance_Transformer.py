import math
import random

def euclidean(p1, p2):
    return math.hypot(p1[0] - p2[0], p1[1] - p2[1])

def read_instance_txt(filename):
    with open(filename, 'r') as f:
        lines = [line.strip() for line in f if line.strip()]

    # Header value (declared n) may be inconsistent with the actual data that follows.
    # Parse header but validate lengths below and prefer the actual number of coordinate lines
    # to avoid silently producing truncated .dat files.
    n_header = int(lines[0])
    risk_threshold = float(lines[1])
    demands = list(map(float, lines[2].split()))
    coords = [tuple(map(float, line.split())) for line in lines[3:]]

    # Validate consistency between header and parsed lists
    len_demands = len(demands)
    len_coords = len(coords)
    if n_header != len_demands or n_header != len_coords or len_demands != len_coords:
        print(f"[Instance_Transformer] WARNING: header n={n_header}, demands_count={len_demands}, coords_count={len_coords}.")
        # Prefer the number of coordinate lines as the authoritative n (most robust for geometric instances)
        # and adjust/truncate/pad the demands list to match. This prevents generating .dat with unexpected index ranges.
        n = len_coords
        if len_demands < n:
            # If some demands are missing, pad with zeros (assume depot/zero-demand) and warn.
            pad = [0.0] * (n - len_demands)
            demands = demands + pad
            print(f"[Instance_Transformer] INFO: padded demands from {len_demands} to {n} with zeros.")
        elif len_demands > n:
            # Truncate extra demand entries to match coordinates.
            demands = demands[:n]
            print(f"[Instance_Transformer] INFO: truncated demands from {len_demands} to {n} to match coords.")
    else:
        n = n_header

    return n, risk_threshold, demands, coords

def write_dat_file(n, risk_threshold, demands, coords, output_filename):
    dat = []

    # Ensure internal n matches provided lists to avoid index errors or truncated outputs.
    if n != len(coords) or n != len(demands):
        print(f"[Instance_Transformer] WARNING: write_dat_file received n={n} but len(coords)={len(coords)}, len(demands)={len(demands)}. Using n=len(coords)={len(coords)} for generation.")
        n = len(coords)

    nodes = list(range(1, n + 1))
    depot_indices = [i + 1 for i, d in enumerate(demands) if d == 0]
    client_indices = [i + 1 for i, d in enumerate(demands) if d > 0]

    # Parámetros dinámicos para viabilidad
    num_vehiculos = max(1, (math.ceil(len(client_indices) / 5)))
    total_demanda = sum(demands[i - 1] for i in client_indices)
    capacidad = math.ceil(total_demanda / num_vehiculos * 1.15)

    # Sets
    dat.append("param sigma\n: 1 2 :=")
    sigma_values = [
        (0.00001, 0.99999), (0.1, 0.9), (0.2, 0.8), (0.3, 0.7), (0.4, 0.6),
        (0.5, 0.5), (0.6, 0.4), (0.7, 0.3), (0.8, 0.2), (0.9, 0.1), (0.99999, 0.00001)
    ]
    for idx, (s1, s2) in enumerate(sigma_values, start=1):
        dat.append(f"{idx}\t{s1}\t{s2}")
    dat.append(";")

    dat.append(f"\nset O := {' '.join(map(str, depot_indices))};")
    dat.append(f"set R := {' '.join(map(str, client_indices))};")
    # Hacer negativos los índices de depósito
    neg_depot_indices = [-i for i in depot_indices]
    dat.append(f"set S := {' '.join(map(str, neg_depot_indices))};")
    dat.append(f"set K := {' '.join(map(str, range(1, (num_vehiculos * len(depot_indices)) + 1)))};")
    # dat.append(f"set S := {' '.join(map(str, depot_indices))};")
    # dat.append(f"set K := {' '.join(map(str, range(1, num_vehiculos + 1)))};")

    # Parámetros clave
    dat.append(f"param b := {capacidad};")
    dat.append(f"param theta := {risk_threshold};")
    # dat.append("param peso_vacio := 3500;")

    # Coeficientes de emisiones (COPERT)
    dat.append("param alpha :=\n0 0\n1 0\n2 0\n3 0\n4 0\n;")
    dat.append("param beta :=\n0 0.2\n1 -0.1\n2 0\n3 0\n4 0\n;")
    dat.append("param gamma :=\n0 16.4\n1 2.2\n2 2.2\n3 0.6\n4 0.4\n;")
    dat.append("param delta :=\n0 0\n1 0\n2 0\n3 0\n4 0\n;")
    dat.append("param epsilon :=\n0 0.3\n1 0\n2 0\n3 0\n4 0\n;")
    dat.append("param zeta :=\n0 2.4\n1 2.5\n2 1.3\n3 4.5\n4 3.3\n;")
    dat.append("param hta :=\n0 1\n1 1\n2 1\n3 1\n4 1\n;")

    # Demandas
    dat.append("param dm :=")
    for i, d in zip(nodes, demands):
        dat.append(f"{i} {int(d)}")
    dat.append(";")

    # Distancias
    dat.append("param d :=")
    for i in range(n):
        for j in range(n):
            dist = euclidean(coords[i], coords[j])
            dat.append(f"{i+1} {j+1} {dist:.2f}")
    dat.append(";")

    # Velocidades (realistas)
    dat.append("param v :=")
    for i in range(n):
        for j in range(n):
            v = 0 if i == j else random.randint(35, 60)
            dat.append(f"{i+1} {j+1} {v}")
    dat.append(";")

    # dat.append("param Rinit :=")
    # for o in depot_indices:
    #     for k in range(1, num_vehiculos + 1):
    #         dat.append(f"{o} {k} 0")
    # dat.append(";")

    # Guardar archivo
    with open(output_filename, 'w') as f:
        f.write('\n'.join(dat))

    print(f"Archivo '{output_filename}' generado correctamente.")

# ---------------------- USO -----------------------

if __name__ == "__main__":
    import argparse
    # instancias = [11, 20, 26, 38, 53, 65, 80, 95, 126, 146, 210, 338]
    instancias = [80]

    parser = argparse.ArgumentParser(description="Convertidor de instancias TXT a archivo .dat para RCMDVRP")
    parser.add_argument("input_file", help="Archivo .txt de instancia")
    parser.add_argument("output_file", help="Nombre del archivo .dat de salida")

    for i in instancias:
        original = f"./Instances/{i}.txt"
        args = parser.parse_args([original, f"./Instances/Instance{i}b.dat"])

        n, risk_threshold, demands, coords = read_instance_txt(args.input_file)
        write_dat_file(n, risk_threshold, demands, coords, args.output_file)
