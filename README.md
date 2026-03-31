# **ESP32: Lipschitz-Delta Thermal Inference**

[[Open In Colab](https://google.com)](https://colab.research.google.com/drive/1j8O1jzEzLknmcNbdoATX470SZstl63fr?usp=sharing)

### **Project Overview**
Implementation of a **Physics-Informed Linear Regressor** designed for recursive temperature forecasting on **ESP32 + DHT11** sensor streams. This system solves the problem of **Recursive Error Propagation** and sensor jitter by enforcing a **Lipschitz-style stability constraint** ($\delta$).

---

### **1. Mathematical Framework**
The model enforces a global "speed limit" on temperature changes to filter non-physical noise:

$$ |\hat{T}_t - \hat{T}_{t-1}| \leq \delta $$

**Stabilization Logic:**
$$ \hat{T}_t = \hat{T}_{t-1} + \text{clip}(\Delta \hat{T}_t^{raw}, -\delta, \delta) $$

**Optimization:**
The optimal $\delta^*$ is found using a **Tri-Point Search** to minimize the Mean Absolute Error (MAE):
$$ \delta^* = \text{argmin}_{\delta > 0} \left( \frac{1}{N} \sum_{i=1}^{N} |T_i - \hat{T}_i(\delta)| \right) $$

---

### **2. Performance Benchmarks**


| Dataset | Characterization | Baseline MAE | **Stabilized MAE** | **Winning Feature** |
| :--- | :--- | :--- | :--- | :--- |
| **D1** | **DHT11 Jitter** | $0.0674$ | **$0.0638$** | **Denoising** |
| **D2** | **High Variance** | $8.142$ | **$4.108$** | **Drift Control** |
| **D3** | **Stochastic Chaos** | $12.803$ | **$12.776$** | **Robustness** |

---

### **3. Key Hardware/Software Stack**
*   **MCU:** ESP32 (Xtensa® Dual-Core)
*   **Sensor:** DHT11 ($0.1^\circ\text{C}$ Resolution)
*   **Language:** Python / C++ (for edge deployment)
*   **Libraries:** Scikit-learn, NumPy, Pandas, Matplotlib

---

### **4. How to Run**
1. Click the **"Open in Colab"** badge above.
2. Upload your `telemetry.csv` or use the provided Kaggle datasets.
3. Run all cells to perform the **Tri-Point Search** and generate stability plots.

---
**Author:** [Your Name]  
**Level:** 3rd Year CSE Engineering Project
