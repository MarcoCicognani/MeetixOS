namespace TC::Functional {

template<typename T, typename E>
Result<T, E>::Result(T const& value)
    : m_value_option{ value } {
}

template<typename T, typename E>
Result<T, E>::Result(T&& value)
    : m_value_option{ Cxx::move(value) } {
}

template<typename T, typename E>
Result<T, E>::Result(E const& error)
    : m_error_option{ error } {
}

template<typename T, typename E>
Result<T, E>::Result(E&& error)
    : m_error_option{ Cxx::move(error) } {
}

template<typename T, typename E>
Result<T, E>& Result<T, E>::operator=(T const& value) {
    m_value_option.reset();
    m_error_option.reset();

    m_value_option = value;
    return *this;
}

template<typename T, typename E>
Result<T, E>& Result<T, E>::operator=(T&& value) {
    m_value_option.reset();
    m_error_option.reset();

    m_value_option = Cxx::move(value);
    return *this;
}

template<typename T, typename E>
Result<T, E>& Result<T, E>::operator=(E const& error) {
    m_value_option.reset();
    m_error_option.reset();

    m_error_option = error;
    return *this;
}

template<typename T, typename E>
Result<T, E>& Result<T, E>::operator=(E&& error) {
    m_value_option.reset();
    m_error_option.reset();

    m_error_option = Cxx::move(error);
    return *this;
}

template<typename T, typename E>
T& Result<T, E>::value() {
    return m_value_option.value();
}

template<typename T, typename E>
T const& Result<T, E>::value() const {
    return m_value_option.value();
}

template<typename T, typename E>
E& Result<T, E>::error() {
    return m_error_option.value();
}

template<typename T, typename E>
E const& Result<T, E>::error() const {
    return m_error_option.value();
}

template<typename T, typename E>
T Result<T, E>::unwrap_value() {
    return m_value_option.unwrap();
}

template<typename T, typename E>
E Result<T, E>::unwrap_error() {
    return m_error_option.unwrap();
}

template<typename T, typename E>
bool Result<T, E>::is_value() const {
    return m_value_option.is_present();
}

template<typename T, typename E>
bool Result<T, E>::is_error() const {
    return m_error_option.is_present();
}

template<typename E>
Result<void, E>::Result(E const& error)
    : m_error_option{ error } {
}

template<typename E>
Result<void, E>::Result(E&& error)
    : m_error_option{ Cxx::move(error) } {
}

template<typename E>
Result<void, E>& Result<void, E>::operator=(E const& error) {
    m_error_option = error;
    return *this;
}

template<typename E>
Result<void, E>& Result<void, E>::operator=(E&& error) {
    m_error_option = Cxx::move(error);
    return *this;
}

template<typename E>
void Result<void, E>::value() {
    /* Only for TRY/MUST compatibility */
}

template<typename E>
void Result<void, E>::value() const {
    /* Only for TRY/MUST compatibility */
}

template<typename E>
E& Result<void, E>::error() {
    return m_error_option.value();
}

template<typename E>
E const& Result<void, E>::error() const {
    return m_error_option.value();
}

template<typename E>
void Result<void, E>::unwrap_value() const {
    /* Only for TRY/MUST compatibility */
}

template<typename E>
E Result<void, E>::unwrap_error() {
    return m_error_option.unwrap();
}

template<typename E>
bool Result<void, E>::is_value() const {
    return !is_error();
}

template<typename E>
bool Result<void, E>::is_error() const {
    return m_error_option.is_present();
}

template<typename T, typename E>
Result<T&, E>::Result(T const& value)
    : m_value_option{ value } {
}

template<typename T, typename E>
Result<T&, E>::Result(T&& value)
    : m_value_option{ Cxx::move(value) } {
}

template<typename T, typename E>
Result<T&, E>::Result(E const& error)
    : m_error_option{ error } {
}

template<typename T, typename E>
Result<T&, E>::Result(E&& error)
    : m_value_option{ Cxx::move(error) } {
}

template<typename T, typename E>
Result<T&, E>& Result<T&, E>::operator=(T const& value) {
    m_value_option.reset();
    m_error_option.reset();

    m_value_option = value;
    return *this;
}

template<typename T, typename E>
Result<T&, E>& Result<T&, E>::operator=(T&& value) {
    m_value_option.reset();
    m_error_option.reset();

    m_value_option = Cxx::move(value);
    return *this;
}

template<typename T, typename E>
Result<T&, E>& Result<T&, E>::operator=(E const& error) {
    m_value_option.reset();
    m_error_option.reset();

    m_error_option = error;
    return *this;
}

template<typename T, typename E>
Result<T&, E>& Result<T&, E>::operator=(E&& error) {
    m_value_option.reset();
    m_error_option.reset();

    m_error_option = Cxx::move(error);
    return *this;
}

template<typename T, typename E>
T& Result<T&, E>::value() {
    return m_value_option.value();
}

template<typename T, typename E>
T const& Result<T&, E>::value() const {
    return m_value_option.value();
}

template<typename T, typename E>
E& Result<T&, E>::error() {
    return m_error_option.value();
}

template<typename T, typename E>
E const& Result<T&, E>::error() const {
    return m_error_option.value();
}

template<typename T, typename E>
T& Result<T&, E>::unwrap_value() {
    return m_value_option.unwrap();
}

template<typename T, typename E>
E Result<T&, E>::unwrap_error() {
    return m_error_option.unwrap();
}

template<typename T, typename E>
bool Result<T&, E>::is_value() const {
    return m_value_option.is_present();
}

template<typename T, typename E>
bool Result<T&, E>::is_error() const {
    return m_error_option.is_present();
}

} /* namespace TC::Functional */