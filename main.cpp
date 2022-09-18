#include <iostream>
#include <limits>
#include <vector>
#include <iomanip>

using namespace std;

// Estructura de un proceso
struct process
{
   int id;              // Identificador del proceso
   int arrival_time;    // Tiempo de llegada
   int burst_time;      // Tiempo de ejecución
   int start_time;      // Tiempo de inicio
   int completion_time; // Tiempo de finalización
   int turnaround_time; // Tiempo de retorno
   int waiting_time;    // Tiempo de espera
   int response_time;   // Tiempo de respuesta
   bool finished;       // Indica si el proceso ha terminado
};

void validate_positive_and_zero_integer_input(int &input);
void input_processes(vector<process> &vector_of_process, int number_of_processes);
void shortest_job_first(vector<process> vector_of_process, int number_of_processes);
void printProcesses(vector<process> vector_of_process, int number_of_processes);
void printAverages(float avg_turnaround_time, float avg_waiting_time, float avg_response_time, float cpu_utilization, float throughput);

/**
   @brief Función principal del programa
   @return 0
**/
int main()
{
   int number_of_processes;           // Número de procesos
   vector<process> vector_of_process; // Vector de procesos

   cout << "Introduce el número de procesos: ";
   cin >> number_of_processes;

   validate_positive_and_zero_integer_input(number_of_processes);

   input_processes(vector_of_process, number_of_processes);

   shortest_job_first(vector_of_process, number_of_processes);

   return 0;
}

/**
   @brief Función para validar la entrada de datos
   @param input Número entero a validar (por referencia)
**/
void validate_positive_and_zero_integer_input(int &input)
{
   while (cin.fail() || input < 0)
   {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Introduce un número válido: ";
      cin >> input;
   }
}

/**
   @brief Función para ingresar los procesos
   @param vector_of_process Vector de procesos (por referencia)
   @param number_of_processes Número de procesos (por valor)
**/
void input_processes(vector<process> &vector_of_process, int number_of_processes)
{
   for (int i = 0; i < number_of_processes; i++)
   {
      process p;

      cout << "Introduce el tiempo de llegada del proceso " << i + 1 << ": ";
      cin >> p.arrival_time;

      validate_positive_and_zero_integer_input(p.arrival_time);

      cout << "Introduce el tiempo de ejecución del proceso " << i + 1 << ": ";
      cin >> p.burst_time;

      validate_positive_and_zero_integer_input(p.burst_time);

      p.id = i + 1;
      p.start_time = 0;
      p.completion_time = 0;
      p.turnaround_time = 0;
      p.waiting_time = 0;
      p.response_time = 0;
      p.finished = false;

      vector_of_process.push_back(p);
   }
}

/**
   @brief Función para ejecutar el algoritmo: primero el trabajo más corto
   @param vector_of_process Vector de procesos (por referencia)
   @param number_of_processes Número de procesos (por valor)
**/
void shortest_job_first(vector<process> vector_of_process, int number_of_processes)
{
   int total_turnaround_time = 0; // Tiempo de retorno total
   int total_waiting_time = 0;    // Tiempo de espera total
   int total_response_time = 0;   // Tiempo de respuesta total
   int total_idle_time = 0;       // Tiempo de inactividad total

   float avg_turnaround_time = 0; // Tiempo de retorno promedio
   float avg_waiting_time = 0;    // Tiempo de espera promedio
   float avg_response_time = 0;   // Tiempo de respuesta promedio

   float cpu_utilization = 0; // Utilización de la CPU
   float throughput = 0;      // Rendimiento

   int completed_processes = 0; // Procesos completados
   int current_time = 0;        // Tiempo actual
   int prev_time = 0;           // Tiempo de inicio del proceso anterior

   while (completed_processes != number_of_processes)
   {
      // Encontrar el proceso listo con el tiempo de ejecución más corto entre los procesos que están en la cola
      int min_burst_time = INT32_MAX;
      int min_burst_time_index = -1;

      for (int i = 0; i < number_of_processes; i++)
      {
         if (vector_of_process[i].finished)
            continue;
         if (vector_of_process[i].arrival_time > current_time)
            continue;
         if (vector_of_process[i].burst_time >= min_burst_time)
            continue;

         min_burst_time = vector_of_process[i].burst_time;
         min_burst_time_index = i;
      }

      // Si ningún proceso es encontrado, incrementa el tiempo actual
      if (min_burst_time_index == -1)
      {
         current_time++;
         continue;
      }

      // Actualizar tiempo de inicio, tiempo de finalización, tiempo de retorno, tiempo de espera, tiempo de respuesta
      vector_of_process[min_burst_time_index].start_time = current_time;
      vector_of_process[min_burst_time_index].completion_time = current_time + vector_of_process[min_burst_time_index].burst_time;
      vector_of_process[min_burst_time_index].turnaround_time = vector_of_process[min_burst_time_index].completion_time - vector_of_process[min_burst_time_index].arrival_time;
      vector_of_process[min_burst_time_index].waiting_time = vector_of_process[min_burst_time_index].turnaround_time - vector_of_process[min_burst_time_index].burst_time;
      vector_of_process[min_burst_time_index].response_time = vector_of_process[min_burst_time_index].start_time - vector_of_process[min_burst_time_index].arrival_time;
      vector_of_process[min_burst_time_index].finished = true;

      // Actualizar tiempos totales
      total_turnaround_time += vector_of_process[min_burst_time_index].turnaround_time;
      total_waiting_time += vector_of_process[min_burst_time_index].waiting_time;
      total_response_time += vector_of_process[min_burst_time_index].response_time;
      total_idle_time += vector_of_process[min_burst_time_index].start_time - prev_time;

      // Actualizar el tiempo actual
      completed_processes++;
      current_time = vector_of_process[min_burst_time_index].completion_time;
      prev_time = current_time;
   }

   // Calcular promedios
   int min_arrival_time = INT32_MAX;
   int max_completion_time = -1;

   for (int i = 0; i < number_of_processes; i++)
   {
      min_arrival_time = min(min_arrival_time, vector_of_process[i].arrival_time);
      max_completion_time = max(max_completion_time, vector_of_process[i].completion_time);
   }

   avg_turnaround_time = (float)total_turnaround_time / number_of_processes;
   avg_waiting_time = (float)total_waiting_time / number_of_processes;
   avg_response_time = (float)total_response_time / number_of_processes;

   // Calcular utilización de la CPU y rendimiento
   cpu_utilization = ((max_completion_time - total_idle_time) / (float)max_completion_time) * 100;
   throughput = float(number_of_processes) / (max_completion_time - min_arrival_time);

   cout << setprecision(2) << fixed;

   printProcesses(vector_of_process, number_of_processes);
   printAverages(avg_turnaround_time, avg_waiting_time, avg_response_time, cpu_utilization, throughput);
}

/**
   @brief Imprimir información de la ejecución
   @param vector_of_process Vector de procesos (por referencia)
   @param number_of_processes Número de procesos (por valor)
**/
void printProcesses(vector<process> vector_of_process, int number_of_processes)
{
   cout << endl;

   for (int i = 0; i < number_of_processes; i++)
   {
      cout << "Proceso " << vector_of_process[i].id << ":" << endl;
      cout << "Tiempo de llegada: " << vector_of_process[i].arrival_time << endl;
      cout << "Tiempo de ejecución: " << vector_of_process[i].burst_time << endl;
      cout << "Tiempo de inicio: " << vector_of_process[i].start_time << endl;
      cout << "Tiempo de finalización: " << vector_of_process[i].completion_time << endl;
      cout << "Tiempo de retorno: " << vector_of_process[i].turnaround_time << endl;
      cout << "Tiempo de espera: " << vector_of_process[i].waiting_time << endl;
      cout << "Tiempo de respuesta: " << vector_of_process[i].response_time << endl;
      cout << endl;
   }
}

/**
   @brief Imprimir promedios
   @param avg_turnaround_time Promedio de tiempo de retorno (por valor)
   @param avg_waiting_time Promedio de tiempo de espera (por valor)
   @param avg_response_time Promedio de tiempo de respuesta (por valor)
   @param cpu_utilization Utilización de la CPU (por valor)
   @param throughput Rendimiento (por valor)
**/
void printAverages(float avg_turnaround_time, float avg_waiting_time, float avg_response_time, float cpu_utilization, float throughput)
{
   cout << "Tiempo de retorno promedio: " << avg_turnaround_time << endl;
   cout << "Tiempo de espera promedio: " << avg_waiting_time << endl;
   cout << "Tiempo de respuesta promedio: " << avg_response_time << endl;
   cout << "Utilización de la CPU: " << cpu_utilization << "%" << endl;
   cout << "Rendimiento: " << throughput << " procesos/unidad tiempo" << endl;
}
