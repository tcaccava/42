'use client';

import { LegalDocumentPage } from '@/components/legal/LegalDocumentPage';
import { PRIVACY_POLICY_SECTION_KEYS } from '@/components/legal/legal-sections';

export default function PrivacyPolicyPage() {
	return <LegalDocumentPage kind="privacy-policy" sections={PRIVACY_POLICY_SECTION_KEYS} />;
}
