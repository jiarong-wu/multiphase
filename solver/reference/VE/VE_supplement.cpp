//supplement info

//coefficient before Pb 
K/mu_b - K/mu_b*Sc/(1-sb_res) + K*krc_star/mu_c * Sc/(1-sb_res)

//coefficient b
K * (1/mu_b - Sc/mu_b/(1-sb_res))

//coefficient c
K * (krc_star*Sc/mu_c/(1-sb_res))

//Pb boundary condition
Pb = rg_b * (Lz - 1/2*dz) // Lz is formation thickness, dz is grid cell size in z direction

//Pcap boundary condition 
Pcap = Pcapentry + ((1-Sc)*Lz-1/2*dz)*(rg_c-rg_b)
Pcapentry = 0

  double get_Pcap(vec &Pcap_solution, vec &old_Sc_solution)
  {
   for (int id = 0; id < CELL_NUMBER; ++id)
     {
      Cell cell(id);

      Pcap_solution(cell.id_) = Pcapentry + ((1-old_Sc_solution(cell.id_))*Lz-1/2*dz)*(rg_c-rg_b);

     }

  }





  //initial condition
Sc = 0
Sb = 1
Pb = rg_b * (Lz - 1/2*dz)
Pcap = Pcapentry + ((1-Sc)*Lz-1/2*dz)*(rg_c-rg_b) 
Pcapentry = 0
Pc = Pb + Pcap
