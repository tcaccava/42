'use client';

import { LegalDocumentPage } from '@/components/legal/LegalDocumentPage';
import { TERMS_SECTION_KEYS } from '@/components/legal/legal-sections';

export default function TermsOfServicePage() {
	return <LegalDocumentPage kind="terms" sections={TERMS_SECTION_KEYS} />;
}
