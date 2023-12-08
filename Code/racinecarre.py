def racine_carree_approximation(nombre, tolerance=1e-10):
    approximation = nombre / 2.0
    
    while abs(approximation**2 - nombre) > tolerance:
        approximation = 0.5 * (approximation + nombre / approximation)
    
    return approximation

nombre = float(input("Entrez un nombre : "))
resultat = racine_carree_approximation(nombre)
print(f"La racine carrée approximative de {nombre} est : {resultat}")
