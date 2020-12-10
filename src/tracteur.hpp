#ifndef TRACTEUR_H
#define TRACTEUR_H


#include <chrono>
#include <math.h>

struct Tracteur {
    std::chrono::system_clock::time_point m_time_received;

    double m_antenne_essieu_avant = 1.5;
    double m_antenne_essieu_arriere = 1.2;
    double m_empatement = 2.7;
    double m_hauteur_antenne = 2;
    double m_antenne_lateral = 0;
    
    double m_outil_distance = 2;
    double m_outil_largeur = 2;
       
    double m_x_antenne = 0;
    double m_y_antenne = 0;
    
    double m_correction_lateral_imu = 0;
    double m_correction_lateral = 0;
    GpsPoint_ptr m_pt_antenne_corrige;
    GpsPoint_ptr m_pt_essieu_arriere;
     
    double m_x_essieu_avant = 0;
    double m_y_essieu_avant = 0;
    
    GpsPoint_ptr m_pt_outil_arriere;
    GpsPoint_ptr m_pt_outil_arriere_gauche;
    GpsPoint_ptr m_pt_outil_arriere_droite;
    
    void setPoint(GpsPoint_ptr p, double ang){
        m_pt_antenne_corrige = p;
        m_x_essieu_avant = m_pt_antenne_corrige->m_x + sin(ang)*m_antenne_essieu_avant;
        m_y_essieu_avant = m_pt_antenne_corrige->m_y + cos(ang)*m_antenne_essieu_avant;
        
        m_pt_essieu_arriere = GpsPoint_ptr(new GpsPoint);
        m_pt_essieu_arriere->m_x = m_pt_antenne_corrige->m_x - sin(ang)*m_antenne_essieu_arriere;
        m_pt_essieu_arriere->m_y = m_pt_antenne_corrige->m_y - cos(ang)*m_antenne_essieu_arriere;
        
        double d_antenne_outil = m_antenne_essieu_arriere+m_outil_distance;
        m_pt_outil_arriere = GpsPoint_ptr(new GpsPoint);
        m_pt_outil_arriere->m_x = m_pt_antenne_corrige->m_x - sin(ang)*(d_antenne_outil);
        m_pt_outil_arriere->m_y = m_pt_antenne_corrige->m_y - cos(ang)*(d_antenne_outil);
        
        m_pt_outil_arriere_droite = GpsPoint_ptr(new GpsPoint);
        m_pt_outil_arriere_droite->m_x = m_pt_antenne_corrige->m_x - sin(ang)*(d_antenne_outil)
            + cos(ang)*m_outil_largeur/2;
         m_pt_outil_arriere_droite->m_y = m_pt_antenne_corrige->m_y - cos(ang)*(d_antenne_outil)
            - sin(ang)*m_outil_largeur/2;
        
        m_pt_outil_arriere_gauche = GpsPoint_ptr(new GpsPoint);
        m_pt_outil_arriere_gauche->m_x = m_pt_antenne_corrige->m_x - sin(ang)*(d_antenne_outil)
            - cos(ang)*m_outil_largeur/2;
        m_pt_outil_arriere_gauche->m_y = m_pt_antenne_corrige->m_y - cos(ang)*(d_antenne_outil)
            + sin(ang)*m_outil_largeur/2;
    }
};

#endif // GPS_FRAMEWORK_H
