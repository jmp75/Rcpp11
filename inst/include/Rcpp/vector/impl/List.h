#ifndef Rcpp__vector__impl_List_h
#define Rcpp__vector__impl_List_h

namespace Rcpp{
        
    #undef VEC
    #define VEC Vector<VECSXP,StoragePolicy>
    
    template <
        template <class> class StoragePolicy
    >
    class Vector<VECSXP,StoragePolicy> :
        public VectorBase< VECSXP, true, VEC>,
        public StoragePolicy<VEC>,
        public SlotProxyPolicy<VEC>,
        public AttributeProxyPolicy<VEC>,
        public NamesProxyPolicy<VEC>,
        public AttributesProxyPolicy<VEC>, 
        public RObjectMethods<VEC>, 
        public VectorOffset<VEC>
    {
    public:
        
        using value_type      = SEXP ;
        using stored_type     = SEXP ;
        using init_type       = SEXP ;
        using Proxy           = internal::generic_proxy<VECSXP> ;
        using const_Proxy     = internal::const_generic_proxy<VECSXP> ;
        using iterator        = internal::Proxy_Iterator<Proxy> ; 
        using const_iterator  = internal::const_Proxy_Iterator<const_Proxy> ;
        using NameProxy       = internal::generic_name_proxy<VECSXP> ;
        using const_NameProxy = internal::generic_const_name_proxy<VECSXP> ;
        
        using VectorOffset<Vector>::size ;
        
        RCPP_GENERATE_CTOR_ASSIGN(Vector)
    
        Vector( SEXP x ) {
            Storage::set__( r_cast<VECSXP>( x ) ) ;
        }
        
        Vector( int n ) : Vector(Rf_allocVector(VECSXP, n) ) {}
        Vector() : Vector(0) {}
    
        template <typename U>
        Vector( int n, const U& obj ) : Vector(Rf_allocVector(VECSXP, n) ) {
            std::fill( begin(), end(), wrap(obj) ) ;
        }
        
        template <bool NA, typename Expr>
        Vector( const SugarVectorExpression<VECSXP,NA,Expr>& other ) : Vector(other.size()) {
            other.apply(*this) ;
        }
    
        template <bool NA, typename Expr>
        Vector& operator=( const SugarVectorExpression<VECSXP, NA, Expr>& expr ){
            int n = expr.size() ;
            if( n != size() ){
                reset(n) ;    
            }
            expr.apply(*this) ;
            return *this ;
        }
        
    private:
        
        inline void reset(int n){
            Storage::set__(Rf_allocVector(VECSXP, n) ) ;        
        }
        
        inline stored_type* data(){
            return reinterpret_cast<stored_type*>( DATAPTR(Storage::get__()) );    
        }
        inline const stored_type* data() const{
            return reinterpret_cast<const stored_type*>( DATAPTR(Storage::get__()) );    
        }
        
    public:
        
        inline iterator begin() { 
            return iterator( Proxy(*this, 0) ); 
        }
        inline iterator end() { 
            return iterator( Proxy(*this, size() ) ); 
        }
        
        inline const_iterator begin() const{ 
            return const_iterator( const_Proxy(*this, 0) );
        }
        inline const_iterator end() const{ 
            return const_iterator( const_Proxy(*this, size() ) );
        }
        
        inline Proxy operator[](int i){ 
            return Proxy(*this, i ) ;
        }
        inline const_Proxy operator[](int i) const { 
            return const_Proxy(*this, size() ) ; 
        }
        
        template <typename... Args> static Vector create(Args... args) {
            return typename create_type<VECSXP, Args...>::type( args... ) ;    
        }
    
        inline NameProxy operator[]( const std::string& name ){
            return NameProxy( *this, name ) ;
        }
        inline const_NameProxy operator[]( const std::string& name ) const {
            return const_NameProxy( *this, name ) ;
        }
    } ;
    
    #undef VEC  
}
#endif