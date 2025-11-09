#!/usr/bin/perl
# **************************************************************
# * Nombres: Diego Mendoza, Juan Diego Ariza
# * Materia: Sistemas Operativos
# * Profesor: John Corredor, Ph.D.
# * Universidad: Pontificia Universidad Javeriana
# * Fecha: Noviembre 11 de 2025
# * Tema: Taller de Evaluación de Rendimiento
# * Descripción: Script para automatizar la ejecución de
# *              experimentos de multiplicación de matrices.
# *************************************************************

$Path = `pwd`;
chomp($Path);

# CONFIGURACIÓN DEL EXPERIMENTO

# CAMBIAR ESTE NOMBRE según el programa que se quiera evaluar:
# Opciones: mmClasicaOpenMP, mmClasicaPosix, mmClasicaFork, mmFilasOpenMP
$Nombre_Ejecutable = "mmClasicaFork";

# Tamaños de matrices a evaluar
@Size_Matriz = ("100", "200", "400", "600", "800", "1000");

# Número de hilos a evaluar
@Num_Hilos = (1, 2, 4, 6, 8);

# Número de repeticiones (MÍNIMO 30 recomendado)
$Repeticiones = 30;

# EJECUCIÓN DE EXPERIMENTOS

$p = 0;
$total_experimentos = scalar(@Size_Matriz) * scalar(@Num_Hilos) * $Repeticiones;

print "==================================================\n";
print "CONFIGURACIÓN DEL EXPERIMENTO\n";
print "==================================================\n";
print "Ejecutable: $Nombre_Ejecutable\n";
print "Tamaños de matrices: @Size_Matriz\n";
print "Número de hilos: @Num_Hilos\n";
print "Repeticiones por config: $Repeticiones\n";
print "Total de ejecuciones: $total_experimentos\n";
print "==================================================\n\n";

foreach $size (@Size_Matriz){
	foreach $hilo (@Num_Hilos) {
		# Construir nombre del archivo de salida
		$file = "$Path/$Nombre_Ejecutable-".$size."-Hilos-".$hilo.".dat";
		
		print "Ejecutando: $Nombre_Ejecutable $size x $size con $hilo hilo(s)...\n";
		
		# Ejecutar las repeticiones
		for ($i=0; $i<$Repeticiones; $i++) {
			# MODO EJECUCIÓN
			system("$Path/$Nombre_Ejecutable $size $hilo  >> $file");
			
			# MODO DEBUG
			# printf("$Path/$Nombre_Ejecutable $size $hilo \n");
		}
		
		close($file);
		$p=$p+1;
		
		# Mostrar progreso
		$porcentaje = int(($p / (scalar(@Size_Matriz) * scalar(@Num_Hilos))) * 100);
		print "   Completado: $p/" . (scalar(@Size_Matriz) * scalar(@Num_Hilos)) . " configuraciones ($porcentaje%)\n\n";
	}
}

print "Total de ejecuciones: $total_experimentos\n";
print "Archivos generados: $p archivos .dat\n";
