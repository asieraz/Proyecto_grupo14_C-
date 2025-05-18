

#ifndef SRC_HEADERS_OBJETOS_H_
#define SRC_HEADERS_OBJETOS_H_

class Producto {
protected:
	int idProd;
	char *nombreProd;
	float precio;
	int codSeccion;
	int codProveedor;
	char *tipo;
	int numDisponibles;
public:
	Producto();
	Producto(int idProd, const char *nombreProd, float precio, int codSeccion, int codProveedor, const char *tipo, int numDisponibles);
	Producto(const Producto &p);
	virtual ~Producto();

	int getIdProd();
	const char *getNombreProd();
	float getPrecio();
	int getCodSeccion();
	int getCodProveedor();
	int getNumDisponibles();

	void setIdProd(int idProd);
	void setNombreProd(const char * nombre);
	void setPrecio(float precio);
	void setCodSeccion(int codSeccion);
	void setCodProveedor(int codProveedor);
	void setNumDisponibles(int numDisponibles);
};

class Proveedor {
protected:
	int codProveedor;
	char *nombreProveedor;
	int codPostal;
	char *contrasena;
public:
	Proveedor();
	Proveedor(int codProveedor, const char *nombreProveedor, int codPostal, const char *contrasena);
	Proveedor(const Proveedor &p);
	virtual ~Proveedor();

	int getCodProveedor();
	const char *getNombreProveedor();
	int getCodPostal();
	const char *getContrasena();

	void setCodProveedor(int codProveedor);
	void setNombreProveedor(const char *nombreProveedor);
	void setCodigoPostal(int codPostal);
	void setContrasena(const char *contrasena);


};

class Seccion {
protected:
	int codSeccion;
	char *nombreSeccion;
public:
	Seccion();
	Seccion(int codSeccion, const char *nombreSeccion);
	Seccion(const Seccion &s);
	~Seccion();

	int getCodSeccion();
	const char *getNombreSeccion();

	void setCodSeccion(int codSeccion);
	void setNombreSeccion(const char *nombreSeccion);
};

class Departamento {
protected:
	int idDepartamento;
	char *nombreDepartamento;
	int NSSJefe;
public:
	Departamento();
	Departamento(int idDepartamento, const char *nombreDepartamento, int NSSJefe);
	Departamento(const Departamento &d);
	~Departamento();

	int getIdDepartamento();
	const char *getNombreDepartamento();
	int getNSSJefe();

	void setIdDepartamento(int idDepartamento);
	void setNombreDepartamento(const char *nombreDepartamento);
	void setNSSJefe(int NSSJefe);
};

class Empleado {
protected:
	int NSS;
	char *nombreEmpleado;
	char *contrasena;
	int codSeccion;
	int idDepartamento;
public:
	Empleado();
	Empleado(int NSS, const char *nombreEmpleado, const char *contrasena, int codSeccion, int idDepartamento);
	Empleado(const Empleado &e);
	~Empleado();

	int getNSS();
	const char *getNombreEmpleado();
	const char *getContrasena();
	int getCodSeccion();
	int getIdDepartamento();

	void setNSS(int NSS);
	void setNombreEmpleado(const char *nombreEmpleado);
	void setContrasena(const char *contrasena);
	void setCodSeccion(int codSeccion);
	void setIdDepartamento(int idDepartamento);
};


#endif /* SRC_HEADERS_OBJETOS_H_ */
