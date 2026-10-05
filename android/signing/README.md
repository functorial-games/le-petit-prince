# Public development identity

This sideload-only test package uses the existing public organization test key
from `isomorphismes/wegert`, revision
`dfa3751f0282cdf6a2083c97063b0e8e996d0d92`,
`_/build/app/wegert-debug.keystore`. No key generation or fallback occurs.

Certificate SHA-256:
`de9b1d47c5a65e6d46a204b79dd9ee566b9d3c9832ba81ebc4213d3392e92ff9`.
Keystore SHA-256:
`d83f7a36a205e49b4c755f37d27a0ded02779acc0da82a22e19d458dc6e5a89a`.
PKCS12; alias/store password/key password `wegert-debug` (public test material).
`test-cert.der` pins the exact expected certificate bytes before signing.
The finished APK certificate is checked independently. Production/store
distribution needs its own identity and is outside this test lane.
