import numpy as np

def add_second_depot_nopython(input_file, output_file):
    # Leer la instancia
    with open(input_file, 'r') as f:
        lines = f.read().splitlines()
    
    num_nodes = int(lines[0])
    risk_threshold = float(lines[1])
    demand_vector = list(map(int, lines[2].split()))
    
    coords = []
    # for line in lines[3:3+num_nodes]:
    #     x, y = map(float, line.split())
    #     coords.append([x, y])
    for line in lines[4:4+num_nodes]:
        x, y = map(float, line.split())
        coords.append([x, y])
    coords = np.array(coords)
    
    # Separar nodos según coordenada Y
    median_y = np.median(coords[:,1])
    cluster_high = coords[coords[:,1] >= median_y]
    cluster_low  = coords[coords[:,1] <  median_y]
    
    depot1 = cluster_high.mean(axis=0)
    depot2 = cluster_low.mean(axis=0)
    
    # Insertar depósito 1 al inicio y depósito 2 al final
    new_coords = [depot1.tolist()] + coords.tolist() + [depot2.tolist()]
    new_demand_vector = demand_vector + [0]
    new_num_nodes = num_nodes + 1
    
    # Guardar nueva instancia
    with open(output_file, 'w') as f:
        f.write(f"{new_num_nodes}\n")
        f.write(f"{risk_threshold}\n")
        f.write(" ".join(map(str, new_demand_vector)) + "\n")
        for x, y in new_coords:
            f.write(f"{x:.3f} {y:.3f}\n")
    
    print(f"Instancia procesada. Guardada en '{output_file}'")

# Ejemplo de uso:
add_second_depot_nopython("./Set S/94.txt", "95.txt")
