using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace JoyfulStickGUI
{
    /// <summary>
    /// Interaction logic for POV.xaml
    /// </summary>
    public partial class POV : UserControl
    {
        public POV()
        {
            InitializeComponent();
        }

        public double Value
        {
            get { return m_Value; }
            set
            {
                m_Value = value;
                if (Value != -1)
                {
                    m_ArrowRotation.Angle = value;
                    m_Arrow.Visibility = Visibility.Visible;
                }
                else
                    m_Arrow.Visibility = Visibility.Hidden;
            }
        }

        public string DisplayName
        {
            get { return m_Name.Content.ToString(); }
            set { m_Name.Content = value; }
        }

        private double m_Value;
    }
}
