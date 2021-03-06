/*
= Sphering stage =

Performs PCA on the selected variables.  

*/

using GLib;

public class GGobi.StageSphere : Stage {
  
  private uint[] selected;
  public uint pcs = 0;
  public bool standardize = true;
  
  public TourMatrix variance;
  public double[] eigen_values;
  public TourMatrix eigen_vectors;
  
  // Select variables and recompute variance
  public void select_vars(uint[] selection) {
    selected.resize(selection.length);
    for(int i = 0; i < selection.length; i++) {
      selected[i] = selection[i];
    }
    
    update_variance();
  }
  
  // Calculate variance-covariance matrix of selected variables
  public void update_variance() {
    int j, i, k, p = selected.length;
    variance = new TourMatrix(p, p);
    double[] stddev = new double[p];
    
    for (k = 0; k < p; k++) {
      for (j = 0; j < p; j++) {
        double value;
        for (i = 0; i < n_rows; i++) {
          value += 
            get_centered_value(i, selected[k]) * 
            get_centered_value(i, selected[j]);
        }
        value /= (double) (n_rows - 1);
        variance.set(j, k, value);
        if (j == k) stddev[k] = Math.sqrt(value);
      }
    }
    
    if (standardize) {
      for (k = 0; k < p; k++)
        for (j = 0; j < p; j++)
          variance.set(j, k, variance.get(j, k) / (stddev[j] * stddev[k]));
    } 
  }
  
  private double get_centered_value(uint i, uint j) {
    double mean = get_variable(j).get_mean();
    return get_raw_value(i, j) - mean;
  }
  
  public void sphere() {
    Svd svd = variance.svd();
    
    eigen_values = svd.d;
    eigen_vectors = svd.U;    
  }
  
  // 
  public TourMatrix project_data(int pcs) {
    int m, j, k, p = selected.length;
    double[] stddev = new double[p];
    for (m = 0; m < n_rows; m++) {
      for (j = 0; j < pcs; j++) {
        double value;
        for (k = 0; k < p; k++) {
          if (standardize) {
            value += eigen_vectors.get(k,j) *              
              get_centered_value(m, selected[k]) / stddev[k];
          } else {
            value += eigen_vectors.get(k,j) *              
              get_centered_value(m, selected[k]);
          }
        }
        /*set_value(m, j, value / eigen_values[j]);*/
      }
    }
    return null;
  }
  
  public double condition(int pcs) {
    return eigen_values[0] / eigen_values[pcs - 1];
  }
  
  public double variance_explained(int pcs) {
    double selected_count, total;

    for (int j = 0; j < selected.length; j++) {
      if (j < pcs) selected_count += eigen_values[j];
      total += eigen_values[j];
    }
    
    return selected_count / total;
  }
 
}