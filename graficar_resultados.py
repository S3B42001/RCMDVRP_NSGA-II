import matplotlib.pyplot as plt
import numpy as np

def leer_final_pop(nombre_archivo):
    soluciones = []
    with open(nombre_archivo, "r") as f:
        for linea in f:
            linea = linea.strip()
            # Ignorar comentarios
            if not linea or linea.startswith("#"):
                continue
            valores = linea.split()
            try:
                # Solo consideramos líneas con floats (objetivos, restricciones, etc.)
                if "e" in valores[0] or "." in valores[0]:
                    obj1 = float(valores[0])  # primer objetivo (distancia)
                    obj2 = float(valores[1])  # segundo objetivo (emisiones)
                    soluciones.append((obj1, obj2))
            except ValueError:
                # Si no son floats, es una línea de rutas (ignorar)
                continue
    return np.array(soluciones)

def graficar_final_pop(soluciones, titulo="Frente de Pareto", archivo_salida="Pareto.png"):
    num_soluciones = len(soluciones)
    
    plt.figure(figsize=(8,6))
    plt.scatter(soluciones[:,0], soluciones[:,1], c="blue", alpha=0.6, edgecolors="k")
    plt.xlabel("Objetivo 1 (Distancia recorrida)")
    plt.ylabel("Objetivo 2 (Emisiones)")
    plt.title(titulo)
    plt.grid(True, linestyle="--", alpha=0.6)
    
    # Añadir texto con el número de soluciones
    plt.text(0.98, 0.98, f'Soluciones: {num_soluciones}', 
             transform=plt.gca().transAxes, 
             fontsize=12, 
             verticalalignment='top',
             horizontalalignment='right',
             bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))

    if archivo_salida:
        plt.savefig(archivo_salida, dpi=300, bbox_inches="tight")
    else:
        plt.show()

if __name__ == "__main__":
    Instances = [11, 20, 26, 38, 53, 65, 80, 95, 126, 146, 210, 338]
    for Instance in Instances:
        # archivo_Final = "./Results_nsga2/final_pop_Instance" + str(Instance) + ".out"
        # soluciones_Final = leer_final_pop(archivo_Final)
        # if soluciones_Final.size > 0:
        #     graficar_final_pop(soluciones_Final, titulo="Soluciones - Instancia " + str(Instance), archivo_salida="./Frentes_Pareto/Soluciones_Instancia" + str(Instance) + ".png")
        # else:
        #     print("⚠ No se encontraron soluciones en el archivo.")

        archivo_Best = "./Results_nsga2/best_pop_Instance" + str(Instance) + ".out"
        soluciones_Best = leer_final_pop(archivo_Best)
        if soluciones_Best.size > 0:
            graficar_final_pop(soluciones_Best, titulo="Frente de Pareto - Instancia " + str(Instance), archivo_salida="./Frentes_Pareto/Pareto_Instancia" + str(Instance) + ".png")
        else:
            print("⚠ No se encontraron soluciones en el archivo.")
